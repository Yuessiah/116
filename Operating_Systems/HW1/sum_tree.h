#include"some_function.h"
#define SUBSIZEMAX 10
static int result[100], resc, cur, ok;
static void sum_tree_init(void) { resc = cur = 0; }

void dfs(int sum, const char* in)
{
	char snum[10];
	int num, len = 0;

	while(in[cur++] != ')') {
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
	static char ans[100];
	int i, j, c = 0;
	sum_tree_init();
	dfs(0, in);

	for(i = 0; i < resc; i++) {
		char s[10];
		strcpy(s, dig_to_str(result[i]));
		for(j = 0; j < strlen(s); j++) ans[c++] = s[j];
		if(i < resc-1) ans[c++] = ','; ans[c++] = ' ';
	}
	ans[c++] = '\n'; ans[c] = '\0';

	return ans;
}
