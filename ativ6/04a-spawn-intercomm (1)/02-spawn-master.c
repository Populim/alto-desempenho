/*
!! a simple Master/slave (dynamically spawnd) example with 
!!  MPI_Comm_spawn, MPI_Comm_get_parent
!!  both Master and slave executables must be build, see slave
*/
//

// Para executar em vários nodes do cluster, rodar, por exemplo:
//  mpirun -np 1 --hostfile halley.txt 01-spawn-simple
// a funcao MPI_Comm_spawn() pode indicar o hostfile tambem.
//
// o numero de processos a gerar é limitado pelo MPI em função do nr de slots.
// Os slots determinam o nr de processos a escalonar em cada processador
// o padrão para o slot é um processo por core, mas pode mudar.
//
// --bynode ou -bynode faz um round-robin por node e nao por core do node
//
// --use-hwthread-cpus if you want Open MPI to default to the number of hardware threads instead of the 
//           number of processor cores
// 
// --oversubscribe  to ignore the number of available slots when deciding the number of processes to launch.
// 
// -host namehost:slots     to determine the number of slots
//
// -host namehost1:slots,namehost2:slots,namehost3:slots (ele trava se não colocar o hostfile no spawn)
//
// 
// para o MPI_Comm_spawn executar sobre diferentes nós, deve-se executar o mpirun com --hostfile ou --host
// isso permite registrar mais slots à execução que serão utilizados no spawn
// veja manpage do mpirun para aplicar corretamente os argumentos https://www.open-mpi.org/doc/v4.0/man1/mpirun.1.php
// veja https://www.mcs.anl.gov/research/projects/mpi/mpi-standard/mpi-report-2.0/node95.htm#Node95
// 

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define NUM_SPAWNS 4

int main(int argc, char **argv)
{
	int  dst = 0, tag = 0, my_rank, num_proc;
	int  array_of_errcodes[10];
	char message_0[] = "hello slave, i'm your master";
	char master_data[] = "slaves to work";
	char message_1[50];
	char slave[25];
	
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;


	MPI_Status      status;
	MPI_Comm        inter_comm;

	
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
	MPI_Get_processor_name(processor_name, &name_len);


	printf("MASTER (%s) : spawning %d slaves ... \n", processor_name, NUM_SPAWNS);

	/* spawn slave and send it a message */  
	strcpy(slave,"./02-spawn-slave");
	MPI_Comm_spawn(slave, MPI_ARGV_NULL, NUM_SPAWNS, MPI_INFO_NULL, dst, MPI_COMM_WORLD, &inter_comm, array_of_errcodes);

	
	printf("MASTER(%s) : send a message to master of slaves (%s) ...\n", processor_name, message_0);
	MPI_Send(message_0, 50, MPI_CHAR, dst , tag, inter_comm);

	MPI_Recv(message_1, 50, MPI_CHAR, dst, tag, inter_comm, &status);
	printf("MASTER (%s) : message received : %s\n", processor_name, message_1);

	MPI_Send(master_data, 50, MPI_CHAR, dst , tag, inter_comm);
	
	
    MPI_Finalize();
	exit(0);
}
