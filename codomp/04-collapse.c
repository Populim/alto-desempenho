//to compile: gcc 04-collapse.c -o 04-collapse -fopenmp -lm
// to run: 04-collapse

# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>
# include <omp.h>

# define I 2
# define K 1000
# define J 1000

int main ( void );
void timestamp ( void );

/******************************************************************************/

int main ( void )
/******************************************************************************/
/*
  Purpose:      MAIN is the main program for MXM_OPENMP. Fill in A and B adn then multiply them.
  Licensing:    This code is distributed under the GNU LGPL license. 
  Modified:     13 October 2011
  Author:       John Burkardt
*/
{
	double angle, a[I][K], b[K][J], c[I][J];
	int i, j, k, thread_num;
	double pi = 3.141592653589793, s, wtime;

	printf ( "\n" );
	printf ( "MXM_OPENMP:\n" );
	printf ( "  C/OpenMP version\n" );
	printf ( "  Compute matrix product C = A * B.\n" );

	thread_num = omp_get_max_threads ( );

	printf ( "\n" );
	printf ( "  The number of processors available = %d\n", omp_get_num_procs ( ) );
	printf ( "  The number of threads available    = %d\n", thread_num );

/*
	Loop 1: Evaluate (fill in) A.
*/


	s = 1.0 / sqrt ( ( double ) ( I ) );
  
	for ( i = 0; i < I; i++ )
	{
		for ( k = 0; k < K; k++ )
		{
			angle = 2.0 * pi * i * k / ( double ) I;
			a[i][k] = (s * ( sin ( angle ) + cos ( angle ) ))* 0 + (i+1);
//   	   printf ("a[%d][%d]=%g ",i, k, a[i][k]);
		}
//    printf("\n");
	}
/*
		Loop 2: Evaluate (fill in) B.
*/

	for ( k = 0; k < K; k++ )
	{
		for ( j = 0; j < J; j++ )
		{
			angle = 2.0 * pi * k * j / ( double ) I;
			b[k][j] = (s * ( sin ( angle ) + cos ( angle ) ))* 0 + (j+1);
	//      printf ("b[%d][%d]=%g ",k, j, b[k][j]);
		}
//  	printf("\n");
	}

	wtime = omp_get_wtime ( );

    # pragma omp parallel shared ( a, b, c) private ( i, j, k )
    {
        //  printf ( "The number of threads reunning = %d\n", omp_get_num_threads() );

/*
		Loop 3: Compute C = A * B.
*/
//		# pragma omp parallel shared ( a, b, c, pi, s ) private ( angle, i, j, k )
//		{
//    	printf ( "The number of threads reunning = %d\n", omp_get_num_threads() );

		# pragma omp for collapse(2)
		for ( i = 0; i < I; i++ )
		{
			for ( j = 0; j < J; j++ )
			{
				c[i][j] = 0.0;
				for ( k = 0; k < K; k++ )
				{
					c[i][j] += a[i][k] * b[k][j];
				}
				//printf ("c(%d,%d)  = %g\n",i, j,a[i][j]);
			}
		}
		//}
	} // end of parallel region

	wtime = omp_get_wtime ( ) - wtime;
	printf ( "Loop 03: Elapsed seconds = %g\n", wtime );
  
// 	for ( i = 0; i < I; i++ )
//  {
//  	for ( j = 0; j < J; j++ )
//      {
//      	printf ("c[%d][%d]=%g ", i, j, c[i][j]);
//      }
//      	printf("\n");
//  }

//	printf ( "  C[%d][%d]  = %g\n", 2, 2, c[2][2] );
/*
  Terminate.
*/
	printf ( "\n" );
	printf ( "MXM_OPENMP:\n" );
	printf ( "  Normal end of execution.\n" );

	printf ( "\n" );

	return 0;
}
