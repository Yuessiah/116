#include<stdio.h>
#include"SPARSE_MATRICES.h"

void mutiply(entry*, int, int, entry*, int, int);

int main()
{
	int an, am, bn, bm;
	scanf("%d%d", &an, &am);
	entry* A = construct(0, 0, 0); //head
	matrix_build(A, an, am);
	input(A, an, am);
	scanf("%d%d", &bn, &bm);
	entry* B = construct(0, 0, 0); //head
	matrix_build(B, bn, bm);
	input(B, bn, bm);

	mutiply(A, an, am, B, bn, bm);

	return 0;
}

void mutiply(entry* A, int an, int am, entry* B, int bn, int bm)
{
	int a[50][50], b[50][50], d[50][50];
	entry *RItr, *CItr;

	RItr = A;
	for(int i = 1; i <= an; i++) {
		RItr = RItr->south;
		CItr = RItr->east;
		for(int j = 1; j <= am; j++) {
			if(CItr != NULL && i == CItr->row && j == CItr->col) {
				a[i][j] = CItr->value;
				CItr = CItr->east;
			} else a[i][j] = 0;
		}
	}

	RItr = B;
	for(int i = 1; i <= bn; i++) {
		RItr = RItr->south;
		CItr = RItr->east;
		for(int j = 1; j <= bm; j++) {
			if(CItr != NULL && i == CItr->row && j == CItr->col) {
				b[i][j] = CItr->value;
				CItr = CItr->east;
			} else b[i][j] = 0;
		}
	}

	for(int i = 1; i <= an; i++)
		for(int j = 1; j <= bm; j++)
			for(int k = 1; k <= bn; k++)
				d[i][j] += a[i][k] * b[j][k];

	//output
	for(int i = 1; i <= an; i++) {
		for(int j = 1; j <= bm; j++) {
			printf("%-3d ", d[i][j]);
		}
		putchar('\n');
	}
}
