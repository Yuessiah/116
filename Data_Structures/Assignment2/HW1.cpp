#include<stdio.h>
const int maxn = 100;
FILE *fptr = NULL;

struct matrix_1_dimension {
	int value;
	int i, j;
}e[maxn*maxn];
int n, D[maxn][maxn], a, b, eLen;

void input()
{
	char fileName[20];
	printf("Please input the file name:");
	scanf("%s", fileName);
	fptr = fopen(fileName, "r");

	fscanf(fptr, "%d", &n);
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			fscanf(fptr, "%d", &D[i][j]);
	fclose(fptr);

	printf("a:");
	scanf("%d", &a);
	printf("b:");
	scanf("%d", &b);
}
void output()
{
	for(int i = 0; i < eLen; i++) {
		char buf[12];
		sprintf(buf, "e[%d]=%d", i, e[i].value);
		printf("%-11sd(%d,%d)\n", buf, e[i].i, e[i].j);
	}
}

int main()
{
	input();

	eLen = 0;
	for(int i = 0; i < n; i++) {
		int jEnd = (b-1)-1+i+1;
		if(jEnd >= n) jEnd = n-1;
		for(int j = (i<=(a-1))? 0 : i-(a-1); j <= jEnd; j++) {
			e[eLen].value = D[i][j];
			e[eLen].i = i; e[eLen].j = j;
			eLen++;
		}
	}

	output();
	return 0;
}
