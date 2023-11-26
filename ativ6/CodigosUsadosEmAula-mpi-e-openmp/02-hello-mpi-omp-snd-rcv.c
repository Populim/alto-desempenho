// para compilar: mpicc 02-hello-mpi-omp-snd-rcv.c -o 02-hello-mpi-omp-snd-rcv -Wall -fopenmp
// para rodar: mpirun -np 2 02-hello-mpi-omp-snd-rcv
// -np pode ser maior
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <omp.h>

#define MAXMESSAGE 64 

#define NT 4  // numero de threads


int  main(int argc, char *argv[])  {
	int npes, myrank, dest, msgtag, ret;
	int i=0, local_i, namelen;
	int thread_number, total_threads;
	
	char bufrecv[MAXMESSAGE], bufsend[MAXMESSAGE];
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	
	MPI_Status status;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &npes);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Get_processor_name(processor_name, &namelen);

	if ( myrank == 0) 
	{
		for (i = 1; i < npes; i++)
		{
			MPI_Recv(bufrecv, MAXMESSAGE, MPI_CHAR, MPI_ANY_SOURCE, 
                         MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			printf("%s\n", bufrecv);
			fflush(0);
		}
	}
	else 
	{
		// OMP sequential region
		#pragma omp parallel private(local_i, total_threads, thread_number) shared(i) num_threads(NT)
		{
			// OMP parallel region
		    total_threads = omp_get_num_threads();
		    thread_number = omp_get_thread_num();
		    #pragma omp critical (adding)
		    {
		      local_i = ++i;
		    }
		    printf("Hello from thread %d/%d, from process %d/%d on %s. i = %d\n", 
			   thread_number, total_threads, myrank, npes, processor_name, local_i);
		}
		// OMP sequential region
		msgtag = 1;
		sprintf(bufsend, "Hello World from process %d with i=%d", myrank, i);
		fflush(0);
//		printf("%s \n", bufsend);
		
		dest = 0;
		MPI_Send(bufsend, strlen(bufsend)+1, MPI_CHAR, dest, msgtag, MPI_COMM_WORLD);
	}
//	fflush(0);

	ret = MPI_Finalize();
	if (ret == MPI_SUCCESS)
		printf("MPI_Finalize success! \n");
	
	return(0);
}
