// para compilar: mpicc 03a-hello-mpi-omp-snds.c -o 03a-hello-mpi-omp-snds -Wall -fopenmp
// para rodar: mpirun -np 2 03a-hello-mpi-omp-snds
// -np pode ser maior
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <omp.h>

#define MAXMESSAGE 64
#define NT 4

int  main(int argc, char *argv[])  
{
	int npes, myrank, dest, msgtag, ret;
	int i=0, local_i, namelen;
	int thread_number, total_threads;
	int provided;
	
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	
	MPI_Status status;
	
	//MPI_Init(&argc, &argv);
	MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE /*MPI_THREAD_MULTIPLE*/, &provided);
	
	// mesmo que MPI_Init(). Somente uma thread executará
	if(provided == MPI_THREAD_SINGLE)
	{
	    printf("MPI_THREAD_SINGLE\n");
	    fflush(0);
	}
	
	// se multithreaded, so processo que chamou MPI_Init_thread 
	// faz chamadas MPI
	if(provided == MPI_THREAD_FUNNELED)
	{
	    printf("MPI_THREAD_FUNNELED\n");
	    fflush(0);
	}

	// se multithreaded, so uma thread faz chamadas MPI por vez.
	if(provided == MPI_THREAD_SERIALIZED)
	{
	    printf("MPI_THREAD_SERIALIZED\n");
	    fflush(0);
	}
	
	// se multithreaded, multiplas threads podem 
	// fazer chamadas MPI por vez sem restricoes.
	if(provided == MPI_THREAD_MULTIPLE)
	{
	    printf("MPI_THREAD_MULTIPLE\n");
	    fflush(0);
	}
	
	MPI_Comm_size(MPI_COMM_WORLD, &npes);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Get_processor_name(processor_name, &namelen);

	if ( myrank == 0) 
	{
		char bufrecv[MAXMESSAGE];

		for (i = 0; i < (npes-1)*NT; i++)  
		{
			MPI_Recv(bufrecv, MAXMESSAGE, MPI_CHAR, MPI_ANY_SOURCE, 
                         MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			printf("Master on processor %s received %s\n", processor_name, bufrecv);
			fflush(0);
		}
	}
	else 
	{
		#pragma omp parallel private(local_i, total_threads, thread_number) shared(i) \
	                             num_threads(NT)
		{
			char bufsend[MAXMESSAGE];

		    total_threads = omp_get_num_threads();
		    thread_number = omp_get_thread_num();
		    #pragma omp critical (adding)
		    {
				local_i = ++i;
		    }
		    printf("Slave: thread %d/%d, from process %d/%d on %s, local_i = %d\n", 
			thread_number, total_threads, myrank, npes, processor_name, local_i);
		    msgtag = 1;
			sprintf(bufsend, "Slave %d, thread %d, local_i=%d", myrank, thread_number, local_i);
//		    printf("%s \n", bufsend);
		    fflush(0);
		
		    dest = 0;
		    MPI_Send(bufsend, strlen(bufsend)+1, MPI_CHAR, dest, msgtag, MPI_COMM_WORLD);
		    fflush(0);
		}  // end of parallel region
	}
//	fflush(0);

	ret = MPI_Finalize();
	if (ret == MPI_SUCCESS)
		printf("MPI_Finalize success! \n");
	
	return(0);
}
