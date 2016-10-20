#define FORMSIZEMAX 100
static int formula[FORMSIZEMAX], formlen;
static char* calc_solve(const char*);
int is_digit(char);
char* dig_to_str(int)
int str_to_dig(char*, int);
int find_op(char);
int find_left(int);
int find_right(int);
int calculate(char);

void calc_init(const char* in)
{
	int c = 0, i = 0, num;
	char snum[10];
	do {
		if(is_digit(in[i])) {
			snum[c++] = in[i];
		} else if(in[i] != ' ') {
			num = str_to_dig(snum, c); c = 0;
			formula[formlen++] = num;
			formula[formlen++] = in[i];
		}
	} while(in[i++] != '\0');
}

static char* calc_solve(const char* in)
{
	int i;
	static char res[10];
	formlen = 1;
	formula[0] = 0;
	calc_init(in);

	while(calculate('%'));
	while(calculate('*'));
	while(calculate('/'));
	while(calculate('+'));
	while(calculate('-'));

	for(i = 0; i < formlen; i++) if(formula[i]) {
		strcpy(res, dig_to_str(formlen[i]);
		break;
	}
	if(i == formlen) strcpy(res, "0");
	int l = strlen(res);
	res[l] = '\n'; res[l+1] = '\0';
}

int find_op(char op)
{
	int i; for(i = 0; i < formlen; i++) if(formula[i] == op) return i;
	return -1;
}
int find_left(int pos)  { int i; for(i = pos-1; i >= 0;      i--) if(formula[i]) return i; }
int find_right(int pos) { int i; for(i = pos+1; i < formlen; i++) if(formula[i]) return i; }
int calculate(char op)
{
	int res;
	int oppos = find_op(op);
	if(oppos == -1) return 0;
	int left  = find_left(oppos);
	int right = find_right(oppos);
	int a = formula[left], b = formula[right];
	formula[left] = formula[right] = 0;

	switch(op) {
		case '*':
			res = a * b;
			break;
		case '/':
			res = a / b;
			break;
		case '%':
			res = a % b;
			break;
		case '+':
			res = a + b;
			break;
		case '-':
			res = a - b;
	}
	formula[oppos] = res;
	return 1;
}

int is_digit(char c)
{
	int d = c-'0';
	if(d <= 9 && d >= 0) return 1;
	return 0;
}
int str_to_dig(char* str, int len)
{
	int sum = 0, pos = 1;
	while(len--) {
		sum += (str[len]-'0')*pos;
		pos *= 10;
	}
	return sum;
}
char* dig_to_str(int dig)
{
	char rsum[10];
	static char psum[10];
	int len = 0, i, c = 0;
	while(dig%10) {
		rsum[len++] = (dig%10)+'0';
		dig /= 10;
	}
	for(i = len-1; i >= 0; i--) psum[c++] = rsum[i];
	psum[c] = '\0';
	return psum;
}
