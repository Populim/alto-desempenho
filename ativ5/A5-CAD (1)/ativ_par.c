#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

//altere aqui conforme o numero de threads da maquina
#define T 8

double f_area(int a, int b){
    return a*b;
}

double f_perimetro(int a, int b){
    return 2*a + 2*b;
}

double f_diag(int a, int b){
   	return sqrt(a*a+b*b);
}

int main(int argc, char* argv[]){
	int n, i;
	scanf("%d", &n);

	int* bases = (int*) malloc(sizeof(int)*n);
	int* alturas = (int*) malloc(sizeof(int)*n);
	double* area = (double*) malloc(sizeof(double)*n);
	double* per = (double*) malloc(sizeof(double)*n);
	double* diag = (double*) malloc(sizeof(double)*n);

	for(int i = 0; i < n; i++)
		scanf("%d%*c", &bases[i]);

	for(int i = 0; i < n; i++)
		scanf("%d%*c", &alturas[i]);

	#pragma omp parallel for num_threads(T) private(i)
		for(i = 0; i < n; i++){
			per[i] = f_perimetro(bases[i], alturas[i]);
			area[i] = f_area(bases[i], alturas[i]);
			diag[i] = f_diag(bases[i], alturas[i]);
		}

	for(int i = 0; i < n; i++){
		printf("base[%d]= %d ", i, bases[i]);
		printf("altura[%d]= %d ", i, alturas[i]);
		printf("per[%d] = %.2lf ", i, per[i]);
		printf("area[%d] = %.2lf ", i, area[i]);
		printf("diag[%d] = %.2lf\n", i, diag[i]);
	}

	free(bases);
	free(alturas);
	free(area);
	free(per);
	free(diag);

	return 0;
}