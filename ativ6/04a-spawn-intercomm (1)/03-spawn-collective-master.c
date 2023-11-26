//compilar: mpicc 03-spawn-collective-master.c -o 03-spawn-collective-master
//executar: mpirun 03-spawn-collective-master -np 1 03-spawn-collective-master
//
// Para executar em vários nodes do cluster, rodar, por exemplo:
//  mpirun -np 1 --hostfile halley.txt binary
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


#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include"mpi.h"

#define NUM_SPAWNS 4

int main(int argc, char **argv)  {
    int  tag = 1, my_rank, num_proc, src, dst, root;
    char msg_0[] = "hello slave, i'm your master";
    char msg_1[50], master_data[] = "slaves to work", slave[40];
    int  errcodes[10], i, vet[NUM_SPAWNS], buf_rcv, vet_master[NUM_SPAWNS];

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    
    

    MPI_Status  	status;
    MPI_Comm    	inter_comm;
    MPI_Request 	mpirequest_mr;
    MPI_Info 		info_param = 0;


    strcpy(slave,"03-spawn-collective-slave");
	
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
	
    MPI_Get_processor_name(processor_name, &name_len);

    for (i = 0; i < NUM_SPAWNS; i++)
		vet[ i ] = i;

    // veja The Info Object aqui: https://www.mcs.anl.gov/research/projects/mpi/mpi-standard/mpi-report-2.0/node53.htm#Node53
    MPI_Info_create(&info_param);

    // para ver quais são as reserved keys
    // acesse https://www.mcs.anl.gov/research/projects/mpi/mpi-standard/mpi-report-2.0/node97.htm#Node97
    MPI_Info_set(info_param, "hostfile", "./halley.txt");


    // manpage do MPI_Comm_spawn>>  https://www.open-mpi.org/doc/v4.0/man3/MPI_Comm_spawn.3.php
    src = dst = root = 0;
    MPI_Comm_spawn(slave, MPI_ARGV_NULL, NUM_SPAWNS, /*MPI_INFO_NULL,*/ info_param, root, MPI_COMM_WORLD, &inter_comm, errcodes);

    // MPI_Info_free(&info_param);

    MPI_Send(msg_0, 50, MPI_CHAR, dst, tag, inter_comm);
    MPI_Irecv(msg_1, 50, MPI_CHAR, src, tag, inter_comm, &mpirequest_mr);
	
    MPI_Send(master_data, 50, MPI_CHAR, dst, tag, inter_comm);
    
    MPI_Scatter(vet, 1, MPI_INT, &buf_rcv, 1, MPI_INT, MPI_ROOT, inter_comm);
    MPI_Gather(vet, 1, MPI_INT, &vet_master, 1, MPI_INT, MPI_ROOT, inter_comm);

    MPI_Wait(&mpirequest_mr, &status);
	
    for (i = 0; i < NUM_SPAWNS; i++) 
		printf("Master (%s): msg_1=%s,vet[%d]=%d,buf_rcv=%d,vet_master[%d]=%d\n",processor_name,msg_1,i,vet[i],buf_rcv,i,vet_master[i]);

    MPI_Finalize();
    exit(0);

}