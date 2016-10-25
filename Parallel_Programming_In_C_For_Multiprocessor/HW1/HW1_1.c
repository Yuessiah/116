#include <mpi.h>
#include <stdio.h>     // printf()
#include <limits.h>    // UINT_MAX

int checkCircuit (int, long);

int main (int argc, char *argv[]) {
	long i;               /* loop variable (64 bits) */
	int id = 0;           /* process id */
	int count = 0;        /* number of solutions */

	int comm_sz;
	long task_itv;

	MPI_Init(NULL, NULL);

	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	task_itv = UINT_MAX / (long)(comm_sz);

	double startTime = 0.0, totalTime = 0.0;
	startTime = MPI_Wtime();

	int sum = 0;
	long begin = id*task_itv;
	long end = begin + task_itv;
	for (i = begin; i <= end; i++) {
		sum += checkCircuit (id, i);
	}
	if(id != 0) MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	else count += sum;

	if(id == 0) {
		int q, recv_cnt;
		for(q = 1; q < comm_sz; q++) {
			MPI_Recv(&recv_cnt, 1, MPI_INT, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			count += recv_cnt;
		}
	}
	
	totalTime = MPI_Wtime() - startTime;
	
	printf("Process %d finished in time %f secs.\n", id, totalTime);
	if(id == 0) printf("\nA total of %d solutions were found.\n\n", count);
	fflush (stdout);

	MPI_Finalize();

	return 0;
}
#define EXTRACT_BIT(n,i) ( (n & (1<<i) ) ? 1 : 0)
#define SIZE 32

int checkCircuit (int id, long bits) {
	int v[SIZE];        /* Each element is a bit of bits */
	int i;

	for (i = 0; i < SIZE; i++) {
		v[i] = EXTRACT_BIT(bits,i);
	}

	if ( ( (v[0] || v[1]) && (!v[1] || !v[3]) && (v[2] || v[3])
				&& (!v[3] || !v[4]) && (v[4] || !v[5])
				&& (v[5] || !v[6]) && (v[5] || v[6])
				&& (v[6] || !v[15]) && (v[7] || !v[8])
				&& (!v[7] || !v[13]) && (v[8] || v[9])
				&& (v[8] || !v[9]) && (!v[9] || !v[10])
				&& (v[9] || v[11]) && (v[10] || v[11])
				&& (v[12] || v[13]) && (v[13] || !v[14])
				&& (v[14] || v[15]) )
			||
			( (v[16] || v[17]) && (!v[17] || !v[19]) && (v[18] || v[19])
				&& (!v[19] || !v[20]) && (v[20] || !v[21])
				&& (v[21] || !v[22]) && (v[21] || v[22])
				&& (v[22] || !v[31]) && (v[23] || !v[24])
				&& (!v[23] || !v[29]) && (v[24] || v[25])
				&& (v[24] || !v[25]) && (!v[25] || !v[26])
				&& (v[25] || v[27]) && (v[26] || v[27])
				&& (v[28] || v[29]) && (v[29] || !v[30])
				&& (v[30] || v[31]) ) )
	{
		printf ("%d) %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d \n", id,
				v[31],v[30],v[29],v[28],v[27],v[26],v[25],v[24],v[23],v[22],
				v[21],v[20],v[19],v[18],v[17],v[16],v[15],v[14],v[13],v[12],
				v[11],v[10],v[9],v[8],v[7],v[6],v[5],v[4],v[3],v[2],v[1],v[0]);

		fflush (stdout);
		return 1;
	} else {
		return 0;
	}
}
