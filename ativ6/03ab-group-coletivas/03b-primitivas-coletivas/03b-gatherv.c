#include <mpi.h>
#include <stdio.h>
#include<stdlib.h>

#define TAM 12
 
int main( int argc, char **argv )
{
    int rank, size,rec_size,i,*vetor_env,*vetor_rec,*vetor_sizes,*vetor_dsp;
//    int rec_size_resto;
 
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
	
    rec_size=TAM/size;
    //rec_size_resto=rec_size+TAM%size;
	
    vetor_env=(int*)malloc(rec_size*sizeof(int));
    vetor_rec=(int*)malloc(TAM*sizeof(int));
    vetor_dsp=(int*)malloc(size*sizeof(int));
    vetor_sizes=(int*)malloc(size*sizeof(int));

    if(rank==0)
    {
        for(i=0;i<size;i++)
		{
            vetor_sizes[i]=rec_size;
            vetor_dsp[i]=i*rec_size;
        }
    }

    for(i=0;i<rec_size;i++)
    {
        vetor_env[i]=(rank*rec_size)+i;
    }
   
    
    MPI_Gatherv (vetor_env,rec_size,MPI_INT,vetor_rec,vetor_sizes,vetor_dsp,MPI_INT,0,MPI_COMM_WORLD);
    if(rank==0)
    {
        for(i=0;i<TAM;i++)
        {
            printf("recebeu %d\n",vetor_rec[i]);
        }
    }
    
    free(vetor_rec);
    free(vetor_env);
    free(vetor_dsp);
    free(vetor_sizes);
    MPI_Finalize();
    
}
