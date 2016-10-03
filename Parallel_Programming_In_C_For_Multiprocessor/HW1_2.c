#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

double pi(int, int);

int main()
{
	srand((unsigned)time(NULL));
	const int number_of_toss = 1<<30;
	int number_in_circle = 0;
	int my_rank, comm_sz;

	MPI_Init(NULL, NULL);

	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	int task_itv = number_of_toss / comm_sz;

	int begin = my_rank * task_itv;
	int end = begin + task_itv;
	int toss, sum = 0;
	for(toss = begin; toss <= end; toss++) {
		double x = (double)rand() / RAND_MAX;
		double y = (double)rand() / RAND_MAX;
		double distance_squared = x*x + y*y;
		if(distance_squared <= 1.0) sum++;
	}

	if(my_rank != 0) MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	else {
		int q;
		for(q = 1; q < comm_sz; q++) {
			int recv_cnt;
			MPI_Recv(&recv_cnt, 1, MPI_INT, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			number_in_circle += recv_cnt;
		}
		number_in_circle += sum;

		printf("Pi is approximately %.16f.\n", pi(number_of_toss, number_in_circle));
	}

	MPI_Finalize();

	return 0;
}

double pi(int NoT, int NiC) { return 4.0 * ((double)NiC / (double)NoT); }
