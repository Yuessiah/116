#include<stdio.h>
#include"SPARSE_MATRICES.h"

void transpose(entry*, int, int);

int main()
{
	int n, m, bn, bm;
	scanf("%d%d", &n, &m);
	entry* A = construct(0, 0, 0); //head
	matrix_build(A, n, m);
	input(A, n, m);

	transpose(A, n, m);

	return 0;
}

void transpose(entry* M, int n, int m)
{
	int a[50][50];
	entry *RItr, *CItr;

	CItr = M;
	for(int i = 1; i <= m; i++) {
		CItr = CItr->east;
		RItr = CItr->south;
		for(int j = 1; j <= n; j++) {
			if(RItr != NULL && j == RItr->row && i == RItr->col) {
				a[i][j] = RItr->value;
				RItr = RItr->south;
			} else a[i][j] = 0;
		}
	}

	//output
	for(int i = 1; i <= m; i++) {
		for(int j = 1; j <= n; j++) {
			printf("%-3d ", a[i][j]);
		}
		putchar('\n');
	}
}
