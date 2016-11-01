#include<stdio.h>
#include<stdlib.h>
#include<time.h>
const int maxn = 40, maxm = 20, maxstep = 50000;
const int imove[] = { -1, 0, 1, 1, 1, 0, -1, -1 };
const int jmove[] = { 1, 1, 1, 0, -1, -1, -1, 0 };
FILE *fptr = NULL;

int n, m, x, y, step;
int count[maxn][maxm];
void input()
{
	printf("N:");
	scanf("%d", &n);
	printf("M:");
	scanf("%d", &m);
	printf("Initial bug position X:");
	scanf("%d", &x);
	printf("Initial bug position Y:");
	scanf("%d", &y);
}
void output()
{
	fptr = fopen("Sample_Output.txt", "w");

	fprintf(fptr, "總共%d步\n", step);
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < m; j++)
			fprintf(fptr, "%-4d", count[i][j]);
		fprintf(fptr, "\n");
	}
	fclose(fptr);
}

bool is_finished()
{
	for(int i = 0; i < n; i++)
		for(int j = 0; j < m; j++)
			if(!count[i][j]) return 0;
	return 1;
}

int main()
{
	srand(time(NULL));
	input();

	for(step = 0; step < maxstep; step++) {
		count[x][y]++;
		if(is_finished()) break;

		int k, xn, yn;
		do {
			k = rand()%8;
			xn = x+jmove[k]; yn = y+imove[k];
		}while(xn < 0 || yn < 0 || xn >= m || yn >= n);

		x = xn; y = yn;
	}

	output();
	return 0;
}
