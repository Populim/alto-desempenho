// para compilar: mpicc slave.c -o slave -Wall
// para rodar: sera inciado pelo mestre com MPI_Comm_spawn
/*
!! a simple/Master slave (dynamically spawnd) examplewith 
!!  MPI_Comm_spawn, MPI_Comm_get_parent
!!  both Master and slave executables must be build, see Master  
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"mpi.h"

int main(int argc, char **argv)  {
	int         tag = 0, my_rank, num_proc;
	char        message_0[50], message_1[] = "hello master :-)" ;
	char        master_data[50], slaves_data[50];

	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;

	MPI_Status		status;
	MPI_Comm		inter_comm; 


	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
	MPI_Get_processor_name(processor_name, &name_len);


	MPI_Comm_get_parent(&inter_comm);

	/* master gets a message from the
	parent (Master) and broadcasts it to the 
	other processes (slaves) */
	
	if ( my_rank == 0 )	
	{
		MPI_Recv(message_0, 50, MPI_CHAR, 0, tag, inter_comm, &status);
		printf("SLAVE nr %d (%s): message received : %s\n", my_rank, processor_name, message_0);
	 
		MPI_Send(message_1, 50, MPI_CHAR,0 , tag, inter_comm);
		MPI_Recv(master_data, 50, MPI_CHAR, 0, tag, inter_comm, &status);

		printf("SLAVE nr %d (%s): master data received : %s\n", my_rank, processor_name, master_data);
		strcpy(slaves_data, master_data);
	}
	
	MPI_Bcast(slaves_data, 50, MPI_CHAR, 0, MPI_COMM_WORLD);
	printf("SLAVE nr %d (%s): slaves data  received from Bcast : %s\n", my_rank, processor_name, slaves_data);

	MPI_Finalize();
	
	exit(0);
	
}