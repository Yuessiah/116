#include<stdio.h>
#include<ctype.h>
#include<math.h>
const int MAXN = 26;

char S[MAXN+10]; //set
int n;
bool selected[MAXN+10];

void printResult()
{
	putchar('{');
	bool printComma = 0;
	for(int i = 0; i < n; i++) if(selected[i]) {
		if(!printComma) printComma = 1;
		else putchar(',');
		putchar(S[i]);
	}
	puts("}");
}

void powerset(int cnt=0)
{
	if(cnt == n) printResult();
	else {
		selected[cnt] = 0;
		powerset(cnt+1);
		selected[cnt] = 1;
		powerset(cnt+1);
	}
}

int main()
{
	char input[MAXN*2+2+10];
	scanf("%s", input);

	for(int i = 1; input[i] != '\0'; i++) {
		if(isalpha(input[i])) S[n++] = input[i];
	}

	powerset();
	printf("Powerset 裡總共有 %d 個集合\n", (int)pow(2, n));

	return 0;
}
