#include"some_function.h"
#define FORMSIZEMAX 100
static int formula[FORMSIZEMAX], formlen;
static char* calc_solve(const char*);
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
	int i, l;
	static char result[10];
	formlen = 1;
	formula[0] = 0;
	calc_init(in);

	while(calculate('%'));
	while(calculate('*'));
	while(calculate('/'));
	while(calculate('+'));
	while(calculate('-'));

	for(i = 0; i < formlen; i++) if(formula[i]) {
		strcpy(result, dig_to_str(formula[i]));
		break;
	}
	if(i == formlen) strcpy(result, "0");
	l = strlen(result);
	result[l] = '\n'; result[l+1] = '\0';

	return result;
}

int find_op(char op)
{
	int i; for(i = 0; i < formlen; i++) if(formula[i] == op) return i;
	return -1;
}
int find_left(int pos)  { int i; for(i = pos-1; i >= 0;      i--) if(formula[i]) return i; return -1; }
int find_right(int pos) { int i; for(i = pos+1; i < formlen; i++) if(formula[i]) return i; return -1; }
int calculate(char op)
{
	int res, oppos, left, right, a, b;
	oppos = find_op(op);
	if(oppos == -1) return 0;
	left  = find_left(oppos);  a = formula[left];
	right = find_right(oppos); b = formula[right];
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
			break;
		default:
			res = 0;
			break;
	}
	formula[oppos] = res;
	return 1;
}
