// para compilar: mpicc hellofor.c -o hellofor -Wall
// para rodar (use uma delas): 
// mpirun -np 4 hellofor   (supondo 04 cores)
// mpirun --use-hwthread-cpus -np 8 hellofor (supondo 4 cores e 2 threads por core)
// mpirun --oversubscribe -np 10 hellofor (sobreposicao)
// mpirun -np 8 --hostfile hosts hellofor (segue o hostfile e deve obedecer limitacoes)
// mpirun -np 8 --host hostname:slots hellofor (segue -host e segue limitacoes. slots é opcional (default 1))
// mpirun --map-by node --hostfile ./halley.txt -np 4 ./hellofor : -np 4 ./hellofor  (espalha processos em nós)
// mpirun --map-by core --hostfile ./halley.txt -np 4 ./hellofor : -np 4 ./hellofor (espalha processos em cores)
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
#define MESSAGE "Hello World running!!!! " 


int  main(int argc, char *argv[])  {
	int npes, myrank, src, dest, msgtag, ret;
	char *bufrecv, *bufsend;
	
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
		
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &npes);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	MPI_Get_processor_name(processor_name, &name_len);

	bufrecv = (char *) malloc (strlen(MESSAGE)+12);
   	msgtag = 1;
	if ( myrank == 0) {
        printf("There are %d processes.\n", npes);
		printf("I am process %d from processor %s. Message with %d char. \n", myrank, processor_name, strlen(MESSAGE));
		//src = dest = 1;
		for (dest = 1; dest<npes; dest ++)
		  MPI_Send(MESSAGE, strlen(MESSAGE)+1, MPI_CHAR, dest, msgtag, MPI_COMM_WORLD);
		
		msgtag = 2;
		printf("Receiving msgs from slaves: ");
		for (src = 1; src < npes; src++) {
			MPI_Recv(bufrecv, strlen(MESSAGE)+12, MPI_CHAR, MPI_ANY_SOURCE, msgtag, MPI_COMM_WORLD, &status);
			printf("(%d)", strlen(bufrecv));
		}
		printf("\n");
	}
	else {
		printf("There are %d processes. I am process %d from processor %s.\n", npes, myrank, processor_name);
		
		bufsend = (char *) malloc (strlen(MESSAGE)+12);
		src = dest = 0;
		MPI_Recv(bufrecv, strlen(MESSAGE)+1, MPI_CHAR, src, msgtag, MPI_COMM_WORLD, &status);
		msgtag = 2;
		sprintf(bufsend, "%s from %d", bufrecv, myrank);
		MPI_Send(bufsend, strlen(bufsend)+1, MPI_CHAR, dest, msgtag, MPI_COMM_WORLD);
		free(bufsend);
	}
	free(bufrecv);
	fflush(0);
	ret = MPI_Finalize();
//	if (ret == MPI_SUCCESS)
//		printf("MPI_Finalize success! \n");
	return(0);
}
