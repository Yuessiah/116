#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<algorithm>
using std::max;

struct LLArray{
  long long m, n, *array;

  LLArray(): m (), n (), array ((long long*)NULL) {}
  LLArray(long long _m, long long _n): m (_m), n(_n), array (new long long[m*n]) {}
  LLArray(LLArray const &rhs): m (rhs.m), n(rhs.n), array (new long long[m*n]) {
    memcpy(array, rhs.array, m*n*sizeof(long long));
  }
  ~LLArray(){ if (array) delete[] array; array=(long long*)NULL; }

  LLArray &operator=(LLArray const &rhs) {
    if (array) delete[] array;
    array = new long long[rhs.m*rhs.n];
    m = rhs.m, n = rhs.n;
    memcpy(array, rhs.array, m*n*sizeof(long long));

    return *this;
  }

  long long const *operator[](long long _n) const {
    return &array[_n*n];
  }

  long long *operator[](long long _n) {
    return &array[_n*n];
  }

  void resizeArray(long long _m, long long _n) {
    LLArray tmp (_m, _n);
    #pragma omp parallel
    {
      #pragma omp for
      for (long long r = 0; r < _m; r++)
        #pragma omp for
        for (long long c = 0; c < _n; c++)
          tmp[r][c] = (r < m && c < n) * (*this)[r][c];
    }

    *this = tmp;
  }
};

FILE *f;

LLArray add(LLArray const &a, LLArray const &b) {
  LLArray c(a.m, b.n);
  #pragma omp parallel
  {
    #pragma omp for
    for (long long y = 0, m = c.m; y < m; ++y)
      #pragma omp for
      for (long long x = 0, n = c.n; x < n; ++x)
        c[y][x] = a[y][x] + b[y][x];
  }

  return c;
}

LLArray sub(LLArray const &a, LLArray const &b) {
  LLArray c(a.m, b.n);
  #pragma omp parallel
  {
    #pragma omp for
    for (long long y = 0, m = c.m; y < m; ++y)
      #pragma omp for
      for (long long x = 0, n = c.n; x < n; ++x)
        c[y][x] = a[y][x] - b[y][x];
  }

  return c;
}

LLArray strassen(LLArray const &a, LLArray const &b) {
  long long n = a.m;
  LLArray c (n, n);

  if(n == 1) { c[0][0] = a[0][0] * b[0][0]; return c; }

  LLArray a11 (n/2, n/2), a12 (n/2, n/2), a21 (n/2, n/2), a22 (n/2, n/2);
  LLArray b11 (n/2, n/2), b12 (n/2, n/2), b21 (n/2, n/2), b22 (n/2, n/2);
  LLArray c11 (n/2, n/2), c12 (n/2, n/2), c21 (n/2, n/2), c22 (n/2, n/2);
  LLArray m1 (n/2, n/2), m2 (n/2, n/2), m3 (n/2, n/2), m4 (n/2, n/2), m5 (n/2, n/2), m6 (n/2, n/2), m7 (n/2, n/2);


  // partition a and b
  #pragma omp parallel
  {
    #pragma omp for
    for (long long y = 0; y < n/2; ++y)
      #pragma omp for
      for (long long x = 0; x < n/2; ++x) {
        a11[y][x] = a[y][x];
        b11[y][x] = b[y][x];
        a12[y][x] = a[y][n/2+x];
        b12[y][x] = b[y][n/2+x];
        a21[y][x] = a[n/2+y][x];
        b21[y][x] = b[n/2+y][x];
        a22[y][x] = a[n/2+y][n/2+x];
        b22[y][x] = b[n/2+y][n/2+x];
      }
  }

  // compute m1 ~ m7
  #pragma omp parallel sections
  {
    #pragma omp section
      m1 = strassen(add(a11, a22), add(b11, b22));
    #pragma omp section
      m2 = strassen(add(a21, a22), b11);
    #pragma omp section
      m3 = strassen(a11, sub(b12, b22));
    #pragma omp section
      m4 = strassen(a22, sub(b21, b11));
    #pragma omp section
      m5 = strassen(add(a11, a12), b22);
    #pragma omp section
      m6 = strassen(sub(a21, a11), add(b11, b12));
    #pragma omp section
      m7 = strassen(sub(a12, a22), add(b21, b22));
  }

  // combine c matrix
  c11 = add(sub(add(m1, m4), m5), m7);
  c12 = add(m3, m5);
  c21 = add(m2, m4);
  c22 = add(sub(add(m1, m3), m2), m6);

  #pragma omp parallel
  {
    #pragma omp for
    for (long long y = 0; y < n/2; ++y)
      #pragma omp for
      for (long long x = 0; x < n/2; ++x) {
        c[y][x] = c11[y][x];
        c[y][n/2+x] = c12[y][x];
        c[n/2+y][x] = c21[y][x];
        c[n/2+y][n/2+x] = c22[y][x];
      }
  }

  return c;
}

int main()
{
  long long mA, nA, mB, nB, n;
  LLArray A, B, C, D;

  // input
  f = fopen("test0", "r");

  fscanf(f, "%lld%lld", &mA, &nA);
  if (mA < 0 || nA < 0) { struct ArrayNegativeSize{} e; throw e; }
  A = LLArray(mA, nA);

  for (long long r = 0; r < mA; ++r)
    for (long long c = 0; c < nA; ++c) fscanf(f, "%lld", &A[r][c]);

  fscanf(f, "%lld%lld", &mB, &nB);
  if (mB < 0 || nB < 0) { struct ArrayNegativeSize{} e; throw e; }
  B = LLArray(mB, nB);

  for (long long r = 0; r < mB; ++r)
    for (long long c = 0; c < nB; ++c) fscanf(f, "%lld", &B[r][c]);

  fclose(f);

  if (nA != mB) { struct ArrayNotMultipliable{} e; throw e; }
  C = LLArray(mA, nB);

  // traditional method
  f = fopen("out1", "w");

  #pragma omp parallel
  {
    #pragma omp for
    for (long long r = 0; r < mA; ++r)
      #pragma omp for
      for (long long c = 0; c < nB; ++c) {
        C[r][c] = 0;
        #pragma omp for
        for (long long k = 0; k < nA; ++k)
          C[r][c] += A[r][k] * B[k][c];
      }
  }

  for (long long r = 0; r < mA; ++r) {
    for (long long c = 0; c < nB; ++c) fprintf(f, "%lld ", C[r][c]);
    fprintf(f, "\n");
  }

  fclose(f);

  // strassen algorithm
  for (n = max(max(mA, nA), max(mB, nB)), ((n & (n-1)) != 0) && (n<<=1); (n & (n-1)) != 0; n &= n-1);

  A.resizeArray(n, n);
  B.resizeArray(n, n);
  C.resizeArray(n, n);
  D = LLArray(n, n);

  f = fopen("out2", "w");

  D = strassen(A, B);

  for (long long r = 0; r < mA; ++r) {
    for (long long c = 0; c < nB; ++c) fprintf(f, "%lld ", D[r][c]);
    fprintf(f, "\n");
  }

  fclose(f);

  return 0;
}
