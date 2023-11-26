// para compilar: gcc vet-estat-par.c -o vet-estat-par -lm -fopenmp
// para executar: vet-estat-par
//
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>


#define N 500000000 //100M

#define NTHREADS 4


double soma(double *vet) 
{
  long long i;
  double sum = .0;

//  printf("Soma in_parallel: %d, thread_num:%d \n", omp_in_parallel(), omp_get_thread_num());
//  fflush(0);
  
  #pragma omp parallel for num_threads(4) private(i) reduction(+:sum)
  for(i = 0; i < N; i++) 
  {
//    printf("soma for thread_num:%d \n", omp_get_thread_num());
//   fflush(0);

    sum = sum + vet[i];
  }
  return(sum);
}

double maximo(double *vet)
{
  long long i;
  double maximum = -1;

//  printf("Maximo in_parallel: %d, thread_num:%d \n", omp_in_parallel(), omp_get_thread_num());
//  fflush(0);

  #pragma omp parallel for num_threads(4) private(i) reduction (max:maximum)
  for( i = 0; i < N; i++)
  {
//    printf("Maximo for thread_num:%d \n", omp_get_thread_num());
//    fflush(0);

    if (vet[i] > maximum)
    {
      maximum = vet[i];
    }
  }
  return(maximum);
}

double minimo(double *vet)
{
  long long i;
  double minimum = (double)999999999;

//  printf("Minimo in_parallel: %d, thread_num:%d \n", omp_in_parallel(), omp_get_thread_num());
//  fflush(0);

  #pragma omp parallel for num_threads(4) private(i) reduction(min:minimum)
  for( i = 0; i < N; i++)
  {
    if (vet[i] < minimum)
    {
      minimum = vet[i];
    }
  }
  return(minimum);
}


double media(double *vet)
{
  long long i;
  double sum = .0, avg = .0;

//  printf("Media in_parallel: %d, thread_num:%d \n", omp_in_parallel(), omp_get_thread_num());
//  fflush(0);

  #pragma omp parallel for num_threads(4) private(i) reduction(+:sum)
  for(i = 0; i < N; i++) 
  {
    sum += vet[i];
  }
  avg = sum / N;

  return(avg);
}


double desviopadrao(double *vet)
{
  long long i;
  double sum = .0, avg = .0, dp= .0, var;

//  printf("Dsv Padrao in_parallel: %d, thread_num:%d \n", omp_in_parallel(), omp_get_thread_num());
//  fflush(0);


  #pragma omp parallel for num_threads(4) private(i) reduction(+:sum)
  for(i = 0; i < N; i++) 
  {
    sum += vet[i];
  }
  avg = sum / N;
//  printf("avg=%g, sum=%g N%d \n", avg, sum, N);
//  fflush(0);

  sum = .0;
  #pragma omp parallel num_threads(4) private(i) reduction(+:sum)
  {
    #pragma omp for
    for(i = 0; i < N; i++)
    {
      sum += pow((vet[i]-avg), 2);
    }
  }
  var = sum / N;
//  printf("var=%g, sum=%g N%d \n", var, sum, N);
//  fflush(0);

  
  dp = sqrt(var);

  return(dp);
}


int main(void) {
	long long i;

  srand(2);
	double *vet;
	double sum, max, min, avg, dp, wtime; 

	omp_set_nested(1);
	
	vet = (double *) malloc(N * sizeof(double));
	if (vet == NULL) {
	  printf ("malloc error com N=%ld \n",N);
	  exit (-1);
	}
	
	printf("Tamanho do Vetor: %ld \n", N);
	fflush(0);
	
	
	for (i = 0; i < N; i++){
    vet[i] = (double) (rand() % 101);
	    //vet[i] = 2*i/N; //N-i;
	}

//	for (i = 0; i < N; i++)
//	{
//	    printf("vet[%d] = %f \n", i, vet[i]);
//	}
	
	wtime = omp_get_wtime ( );
	
	#pragma omp parallel sections num_threads(NTHREADS) private(i) shared(vet)
	{
	  #pragma omp section
	  {
//		  printf(">>>>>soma threadid=%d \n ", omp_get_thread_num());
	      sum = soma(vet);
	  }

	  #pragma omp section
	  {
//		  printf(">>>>>maximo threadid=%d \n ", omp_get_thread_num());
		  max = maximo(vet);
	  }

	  #pragma omp section
	  {
//		printf(">>>>>minimo threadid=%d \n ", omp_get_thread_num());
		min = minimo(vet);
	  }

	  #pragma omp section
	  {
//		  printf(">>>>>media threadid=%d \n ", omp_get_thread_num());
	      avg = media(vet);
	  }
	  #pragma omp section 
	  {
//		  printf(">>>>>desvio padrão threadid=%d \n ", omp_get_thread_num());
	      dp = desviopadrao(vet);
	  } 
	}

	wtime = omp_get_wtime ( ) - wtime;
	  
	printf("Soma=%.3f, Máximo=%.3f, Mínimo=%.3f, Media=%.3f, Dsv Padrão=%.3f \nWTime:%.3f \n", sum, max, min, avg, dp, wtime);
	fflush(0);

	return(0);

} // main()
