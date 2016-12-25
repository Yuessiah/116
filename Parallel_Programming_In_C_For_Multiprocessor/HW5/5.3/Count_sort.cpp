#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
const int maxn = 100;
int thread_count;

void Count_sort(int a[], int n) {
	int count;
	int* temp = (int*)malloc(n*sizeof(int));

#pragma omp parallel for private(count) num_threads(thread_count)
	for(int i = 0; i < n; i++) {
		//int tid = omp_get_thread_num();
		//printf("tid = %d, i = %d\n", tid, i);
		count = 0;
		for(int j = 0; j < n; j++) {
			//int ttid = omp_get_thread_num();
			//printf("ttid = %d, j = %d\n", ttid, j);
			if(a[j] < a[i] || a[j] == a[i] && j < i) count++;
		}
		temp[count] = a[i];
	}

	memcpy(a, temp, n*sizeof(int));
	free(temp);
}

int main(int argc, char* argv[])
{
	int a[maxn], n;

	printf("Input the n: ");
	scanf("%d", &n);
	printf("Input the sequence: ");
	for(int i = 0; i < n; i++) scanf("%d", a+i);

	thread_count = atoi(argv[1]);
	Count_sort(a, n);

	for(int i = 0; i < n; i++) printf("%d ", a[i]);
	puts("");

	return 0;
}
