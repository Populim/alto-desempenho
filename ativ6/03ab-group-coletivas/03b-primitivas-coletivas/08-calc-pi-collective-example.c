// calculating PI using collective communication with ..
// .. MPI_Bcast, MPI_Gather, MPI_Reduce

// program collective

// author unknown

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

// function to integrate
#define f(a) 4.0/(1.0 + a*a)
#define TRUE 1

int	main(int  argc,char** argv)
{
     int          myrank, nprocs, n, i, islave, master=0, count;
     MPI_Status   status;
     int          ierr, resultlen, tag;
     char         hostname[MPI_MAX_PROCESSOR_NAME];
     double       t1, t2;
     double       PI25DP = 3.141592653589793238462643;
     double       mypi;
     double       f, a, h, s, x, pi, sum;
     double*      partial_results;

     // Initialize MPI
     MPI_Init(&argc, &argv);

     // Get my rank id and number of MPI processes
     MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
     MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

     // Get hostname
     MPI_Get_processor_name(hostname, &resultlen);
     printf("Rank %d out of %d processes running on %s\n",myrank, nprocs, hostname);

     //only the master needs this array
     if ( myrank == master )
            partial_results = (double *) calloc(nprocs,sizeof(double)) ;

     // Synchronization point (not really necessary here)
     MPI_Barrier (MPI_COMM_WORLD);

  //!-----------------------------------------------------------
  //! Do work here ...

    // The master sets an integer number form standard input and sends it to all slaves
    if ( myrank == master )
    {   // Master part
	// printf("Number of intervals ? (0 quits): \n");
	// scanf("%d",&n);
	n = 50000000;  //50M
	t1 = MPI_Wtime();
    }

    MPI_Bcast (&n, 1, MPI_INT, master, MPI_COMM_WORLD);
//    if ( (n <= 0) || (n > 100000000) )
//    	break;

     h  = 1.0/ (double) n; // even step size h as a function of partions
     s  = 0.0;             // zeroise sum

     for(i=myrank+1; i<=n; i+=nprocs) 
     {  // Simple approach of work sharing
        // Salto a cada iter é de nprocs itens => (i+=nprocs)
        // Por isso inicia o i de cada processo em myrank+1
        x = h * ( (double)i - 0.5);
        s = s + f(x);
     }

     mypi = h * s;

     // solution 1: all slaves send their results to the master ..
     MPI_Gather ( &mypi, 1, MPI_DOUBLE, partial_results, 1, MPI_DOUBLE, master, MPI_COMM_WORLD);
     // .. who than builds the final sum
     if ( myrank == 0 )
     {
        sum=0.0;
     	printf("partial results :");
        for(i=0;i<nprocs;i++)
        {
            printf(" %lf, ",partial_results[i]);
            sum = sum + partial_results[i];
        }
        printf("\n");

        printf("final sum : %lf\n", sum);
     }

     // solution 2: adding up all partial results is done collectively
     MPI_Reduce(&mypi,&pi,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD); // collect all the sums

     if ( myrank == 0 )
     {
        t2 = MPI_Wtime();
        printf("pi is approximately: %18.16f Error is %18.16f\n",pi,fabs(pi-PI25DP));
        printf("process %d : %lf seconds\n", myrank,t2-t1);
     }

    //Shut Down MPI
    MPI_Finalize();

  //end program collective
}
