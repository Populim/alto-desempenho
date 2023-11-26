#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define N 10
int main(){

	double *data = (double *)malloc(N*N*sizeof(double));
	double **array= (double **)malloc(N*sizeof(double*));
	for (int i=0; i<N; i++)
		array[i] = &(data[N*i]);

	for (int i = 0; i < N*N; i++){
		data[i] = i;
	}

	for (int i = 0; i < N*N; i++){
		printf("%g ",data[i]);
	}
	printf("\n");



	printf("ponteiro do data: %p\n", data);
	printf("ponteiros do array: %p %p %p\n", array[0], array[1], array[2]);


	free(data);
	free(array);

	sleep(10);

	printf("ponteiro do data: %p\n", data);
	printf("ponteiros do array: %p %p %p\n", array[0], array[1], array[2]);
	

	for (int i = 0; i < N*N; i++){
		printf("%g ",data[i]);
	}
	printf("\n");

}