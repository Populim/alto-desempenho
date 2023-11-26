// to compile: mpicc 01-spawn-simple.c -o 01-spawn-simple
// to run: mpirun -np 1 01-spawn-simple
//
// Para executar em vários nodes do cluster, rodar, por exemplo:
//  mpirun -np 1 --hostfile halley.txt 01-spawn-simple
// a funcao MPI_Comm_spawn() pode indicar o hostfile tambem.
//
//
// A aplicação funciona com -np maiores que 1. Permite exemplificar que se trata de uma primitiva coletiva 
// e que apenas um processo cria os processos filhos.
// to run: mpirun -np 8 01-spawn-simple
//
// o numero de processos a gerar é limitado pelo MPI em função do nr de slots.
// Os slots determinam o nr de processos a escalonar em cada processador
// o padrão para o slot é um processo por core, mas pode mudar.
//
// --map-by node faz um round-robin por node e nao por core do node
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

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// o numero de processos a gerar é limitado pelo MPI em função 
// de fatores como, por exemplo, o número de nucleos nos processadores
// neste caso só um processador é usado).
#define NUM_SPAWNS 8 

int main( int argc, char *argv[] )
{
    int my_rank;
    int errcodes[NUM_SPAWNS];
    
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
	
    MPI_Comm parentcomm, intercomm;

    MPI_Init( &argc, &argv );
    MPI_Comm_get_parent( &parentcomm );
    MPI_Get_processor_name(processor_name, &name_len);
  
    
    if (parentcomm == MPI_COMM_NULL)
    {
		MPI_Info  info_param;
		int root = 0;

		MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

//		printf("01-spawn-simple: Parent running!\n");
//		fflush(0);

		// veja The Info Object aqui: https://www.mcs.anl.gov/research/projects/mpi/mpi-standard/mpi-report-2.0/node53.htm#Node53
		MPI_Info_create(&info_param);

		// para ver quais são as reserved keys
		// acesse https://www.mcs.anl.gov/research/projects/mpi/mpi-standard/mpi-report-2.0/node97.htm#Node97
		MPI_Info_set(info_param, "hostfile", "./halley.txt");


		// manpage do MPI_Comm_spawn>>  https://www.open-mpi.org/doc/v4.0/man3/MPI_Comm_spawn.3.php
        /* Create 2 more processes - this example must be called spawn_example.exe for this to work. */
        MPI_Comm_spawn( "01-spawn-simple", MPI_ARGV_NULL, NUM_SPAWNS, MPI_INFO_NULL, /*info_param, */root, MPI_COMM_WORLD, &intercomm, errcodes );
        printf("I'm the parent number %d on processor %s.\n", my_rank, processor_name);
		fflush(0);
    }
    else
    {
		MPI_Comm_rank(parentcomm, &my_rank);
        printf("I'm the spawned process number %d on processor %s.\n", my_rank, processor_name);
    }
    
    fflush(stdout);
    MPI_Finalize();
    
    return 0;
}

