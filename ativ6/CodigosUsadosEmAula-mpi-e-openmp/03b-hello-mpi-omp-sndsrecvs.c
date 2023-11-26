// para compilar: mpicc 03b-hello-mpi-omp-sndsrecvs.c -o 03b-hello-mpi-omp-sndsrecvs -fopenmp
// para rodar: mpirun -np 2 03b-hello-mpi-omp-sndsrecvs
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
	int i, local_i, namelen;
	int thread_number, total_threads;
	int provided;
	
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	
	MPI_Status status;
	
	MPI_Init_thread(&argc, &argv, /*MPI_THREAD_SINGLE */MPI_THREAD_MULTIPLE, &provided);
	
	// mesmo que MPI_Init(). Somente uma thread executará
	// neste codigo em particular trava.
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

	i = myrank;
	
	if ( myrank == 0) 
	{
		#pragma omp parallel num_threads(NT) private(total_threads, thread_number, msgtag)
		{
			char bufrecv[MAXMESSAGE];
			MPI_Status status;
			int src;

		    total_threads = omp_get_num_threads();
		    msgtag = thread_number = omp_get_thread_num();

		    printf("Master on %s, process %d/%d, thread %d/%d msgtag %d\n", 
			processor_name, myrank, npes, thread_number, total_threads, msgtag);
			fflush(0);

			// a thread T do processo mestre recebe as mensagens das threads T dos processos
			// escravos. Por isso src varia de 1 até npes-1 (i.e., src<npes).
			// essa associacao das threads de mesmo nr eh garantida pela msgtag
			// observe que sao recebidas NT msgs de cada um dos npes-1 processos,
			// pois este for esta em uma regiao paralela do omp
			for (src = 1; src < npes; src++)
			{
				MPI_Recv(bufrecv, MAXMESSAGE, MPI_CHAR, src, msgtag, MPI_COMM_WORLD, &status);
				printf("Master on:%s, thread:%d/%d, received msg:%s\n", processor_name, thread_number, total_threads, bufrecv);
				fflush(0);
			}
		} // end of parallel region
	}
	else 
	{
		// (npes-1) processos MPI executam este else
	    dest = 0;
		#pragma omp parallel private(local_i, total_threads, thread_number, msgtag) shared(i, dest) \
	                             num_threads(NT)
		{
			//(npes-1)*NT threads executam este codigo, sendo
			// NT threads para cada processo MPI
			char bufsend[MAXMESSAGE];

		    total_threads = omp_get_num_threads();
		    msgtag = thread_number = omp_get_thread_num();
		    #pragma omp critical (adding)
		    {
				local_i = i++;
		    }
		    printf("Slave on %s, process %d/%d, thread %d/%d, local_i=%d\n", 
			processor_name, myrank, npes, thread_number, total_threads, local_i);
			sprintf(bufsend, "Slave:%d on %s, thread %d/%d, local_i=%d", myrank, processor_name, thread_number, total_threads, local_i);
//		    printf("%s \n", bufsend);
		    fflush(0);
	
			// cada thread T de cada processo escravo envia uma mensagem para a 
			// respectiva thread T do processo mestre (myrank ==0). Isso eh feito
			// com a msgtag recebendo o nr da thread no processo.
		    MPI_Send(bufsend, strlen(bufsend)+1, MPI_CHAR, dest, msgtag, MPI_COMM_WORLD);
		    fflush(0);
		}// end of parallel region
	} // end of else
//	fflush(0);

	ret = MPI_Finalize();
	if (ret == MPI_SUCCESS)
		printf("MPI_Finalize success! \n");
	
	return(0);
}
