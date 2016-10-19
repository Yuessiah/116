#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define SUBSIZEMAX 10
static int result[100], resc = 0, cur = 0, ok;
static int str_to_dig(char*, int);
static int is_digit(char);

void dfs(int sum, const char* in)
{
	char snum[10];
	int num, len = 0;

	printf("sum = %d\n", sum);
	while(in[cur++] != ')') {
		printf("cur = %d, in[cur] = %c\n", cur, in[cur]);
		if(is_digit(in[cur])) snum[len++] = in[cur];
		else {
			num = str_to_dig(snum, len); len = 0;
			if(in[cur] == ' ' || in[cur] == ')') ok = 1;
			if(in[cur] == '(') dfs(sum+num, in);
			if(ok) { result[resc++] = sum+num; ok = 0; }
		}
	}
}

static char* sum_tree_solve(const char* in)
{
	dfs(0, in);

	int i;
	for(i = 0; i < resc; i++) printf(", %d", result[i]);
	putchar('\n');
}

static int str_to_dig(char* str, int len)
{
	int sum = 0, pos = 1;
	while(len--) {
		sum += (str[len]-'0')*pos;
		pos *= 10;
	}
	return sum;
}
static int is_digit(char c)
{
	int d = c-'0';
	if(d <= 9 && d >= 0) return 1;
	return 0;
}

int main()
{
	char s[100];
	fgets(s, 100, stdin);
	sum_tree_solve(s);

	return 0;
}
