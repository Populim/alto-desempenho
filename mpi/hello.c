// para compilar: mpicc hello.c -o hello -Wall
// para rodar experimente uma destas:
// mpirun -np 2 hello   (supondo 04 cores)
// mpirun --oversubscribe -np 2 hello (sobreposicao)
// mpirun -np 2 --hostfile hosts hello (segue o hostfile e deve obedecer limitacoes)
// mpirun -np 2 --host hostname:slots hello (segue -host e segue limitacoes. slots é opcional (default 1))
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
#define MESSAGE "Hello World running!!!! " 
int  main(int argc, char *argv[])  {
	int npes, myrank, src, dest, msgtag, ret=0;
	char *bufrecv, *bufsend;
	MPI_Status status;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &npes);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	
	bufrecv = (char *) malloc (strlen(MESSAGE)+1);
   	msgtag = 1;
	if ( myrank == 0) {
        //printf("There are %d processes.\n", npes);
		//printf("I am process %d. Message with %d char. \n", myrank, strlen(MESSAGE));
		//fflush(0);
		src = dest = 1;
		MPI_Send(MESSAGE, strlen(MESSAGE)+1, MPI_CHAR, dest, msgtag, MPI_COMM_WORLD);
		msgtag = 2;
		MPI_Recv(bufrecv, strlen(MESSAGE)+1, MPI_CHAR, src, msgtag, MPI_COMM_WORLD, &status);
		printf("Received message from slave: %s with %d char \n", bufrecv, (int)strlen(bufrecv));
	}
	else {
        //printf("There are %d processes. I am process %d \n", npes, myrank);
		//fflush(0);
        bufsend = (char *) malloc (strlen(MESSAGE)+1);
		src = dest = 0;
		MPI_Recv(bufrecv, strlen(MESSAGE)+1, MPI_CHAR, src, msgtag, MPI_COMM_WORLD, &status);
		msgtag = 2;
		strcpy(bufsend, bufrecv);
		MPI_Send(bufsend, strlen(MESSAGE)+1, MPI_CHAR, dest, msgtag, MPI_COMM_WORLD);
		free(bufsend);
	} 
	free(bufrecv);
	fflush(0);
	ret = MPI_Finalize();
	if (ret == MPI_SUCCESS)
		printf("MPI_Finalize success! \n");
	return(0);
}
