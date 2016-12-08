#ifndef _SPARSE_MATRICES_
#define _SPARSE_MATRICES_

#include<stdlib.h>
#include<stdio.h>

struct entry;
entry* construct(int, int, int);
void east_push(entry*, entry*);
void south_push(entry*, entry*);
void matrix_build(entry*, int, int);
void input(entry*, int, int);
void print(entry*, int, int);

struct entry {
	int row, col, value;
	entry *east, *south;
};

entry* construct(int r, int c, int v)
{
	entry* E = (entry*)malloc(sizeof(entry));
	E->row = r;
	E->col = c;
	E->value = v;
	E->east = E->south = NULL;
	return E;
}

void east_push(entry* A, entry* B)
{
	entry* Itr = A;
	while(Itr->east != NULL) Itr = Itr->east;
	Itr->east = B;
}

void south_push(entry* A, entry* B)
{
	entry* Itr = A;
	while(Itr->south != NULL) Itr = Itr->south;
	Itr->south = B;
}

void matrix_build(entry* M, int n, int m) {
	entry* CItr = M;
	for(int c = 1; c <= m; c++) {
		entry* Col = construct(0, c, 0);
		east_push(CItr, Col);
		CItr = CItr->east;
	}
	CItr->east = M;

	entry* RItr = M;
	for(int r = 1; r <= n; r++) {
		entry* Row = construct(r, 0, 0);
		south_push(RItr, Row);
		RItr = RItr->south;
	}
	RItr->south = M;
}

void input(entry* M, int n, int m)
{
	entry* RItr = M;
	for(int r = 1; r <= n; r++) {
		RItr = RItr->south;
		entry* CItr = M;
		for(int c = 1; c <= m; c++) {
			CItr = CItr->east;
			int v; scanf("%d", &v);
			if(!v) continue;
			entry* E = construct(r, c, v);
			east_push(RItr, E);
			south_push(CItr, E);
		}
	}
}

#endif
