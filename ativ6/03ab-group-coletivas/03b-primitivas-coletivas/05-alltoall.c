#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

 
int main( int argc, char **argv )
{
    int rank, size,rec_size, i,*vetor_env,*vetor_rec;
 
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    rec_size = size;
    vetor_env=(int*)malloc(rec_size*sizeof(int));
    vetor_rec=(int*)malloc(rec_size*sizeof(int));

    for(i = 0; i < rec_size; i++)
    {
        vetor_env[i] = (rank*rec_size)+i;
    }
        
    MPI_Alltoall (vetor_env, 1, MPI_INT, vetor_rec, 1, MPI_INT, MPI_COMM_WORLD);
    
    for(i=0;i<rec_size;i++)
    {
        printf("O processo %d recebeu %d\n", rank, vetor_rec[i]);
    }
    
    
    free(vetor_rec);
    free(vetor_env);
    MPI_Finalize();
}
