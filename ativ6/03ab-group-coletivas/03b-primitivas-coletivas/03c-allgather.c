#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define TAM 12
 
int main( int argc, char **argv )
{
    int rank, size,rec_size, i,*vetor_env,*vetor_rec;
 
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    rec_size=TAM/size;
    vetor_env=(int*)malloc(TAM*sizeof(int));
    vetor_rec=(int*)malloc(TAM*sizeof(int));

    for(i=0;i<rec_size;i++)
    {
        vetor_env[i]=(rank*rec_size)+i;
    }
        
    MPI_Allgather (vetor_env,rec_size,MPI_INT,vetor_rec,rec_size,MPI_INT,MPI_COMM_WORLD);
    for(i=0;i<TAM;i++)
    {
        printf("O processo %d recebeu %d\n",rank,vetor_rec[i]);
    }
    
    
    free(vetor_rec);
    free(vetor_env);
    MPI_Finalize();
}
