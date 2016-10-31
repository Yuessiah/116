#include<mpi.h>
#include<algorithm>
#include<stdio.h>
using std::swap;

const int maxn = 100;
int L[maxn], n;
int my_rank, comm_sz, task_itv;
void compare_exchage_for(bool, int, int);

int main()
{
	MPI_Init(NULL, NULL);

	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	if(my_rank == 0) {
		printf("n = ");
		scanf("%d", &n);
		printf("list = ");
		for(int i = 0; i < n; i++) scanf("%d", &L[i]);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(L , n, MPI_INT, 0, MPI_COMM_WORLD);

	task_itv = (my_rank != comm_sz-1)? n/comm_sz : n-(n/comm_sz)*(comm_sz-1);
	int start = my_rank*(n/comm_sz);
	int end = start+task_itv - 1;

	for(int i = 0; i < n; i++) {
		compare_exchage_for(i&1, start, end);
		MPI_Barrier(MPI_COMM_WORLD);
	}

	MPI_Gather(L+start, task_itv, MPI_INT, L, task_itv, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);

	if(my_rank == 0) {
		for(int i = 0; i < n; i++) printf("%d ", L[i]);
		putchar('\n');
	}

	MPI_Finalize();
}

void compare_exchage_for(bool phase, int start, int end)
{
	int endAddOne;
	if(!((start&1)==phase) && my_rank > 0) {
		MPI_Send(L+start, 1, MPI_INT, my_rank-1, 1, MPI_COMM_WORLD);
		MPI_Recv(L+start, 1, MPI_INT, my_rank-1, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	if(!((end&1)!=phase) && my_rank != comm_sz-1) {
		MPI_Recv(&endAddOne, 1, MPI_INT, my_rank+1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		//printf("endAddOne = %d\n", endAddOne);
		if(L[end] > endAddOne) swap(L[end], endAddOne);
		MPI_Send(&endAddOne, 1, MPI_INT, my_rank+1, 2, MPI_COMM_WORLD);
	}

	for(int i = start+!((start&1)==phase); i < end; i+=2)
		if(L[i] > L[i+1]) swap(L[i], L[i+1]);
}
