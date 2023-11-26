#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define TAM 16
 
int main( int argc, char **argv )
{
    int rank, size,rec_size, i,*vetor_env,*vetor_rec;
 
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    MPI_Request request;
    MPI_Status status;

    rec_size=TAM/size;
    vetor_env=(int*)malloc(TAM*sizeof(int));
    vetor_rec=(int*)malloc(rec_size*sizeof(int));

    if(rank==0)
    {
        for(i=0;i<TAM;i++)
        {
            vetor_env[i]=i;
        }
        
    }

    MPI_Iscatter (vetor_env,rec_size,MPI_INT,vetor_rec,rec_size,MPI_INT,0,MPI_COMM_WORLD, &request);

    // faz qq outra coisa enquanto nao terminou...
    
    MPI_Wait(&request, &status); // agora espera terminar a comm coletiva nao bloqueante
    
    for(i=0;i<rec_size;i++)
    {
        printf("Rank :%d recebeu %d\n",rank, vetor_rec[i]);
    }
    
    free(vetor_env);
    free(vetor_rec);
    
    MPI_Finalize();
    
}
