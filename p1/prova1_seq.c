#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double** alocaMatrizDouble(double** M,int n){
	M = (double**)malloc(sizeof(double*)*n);
	for (int i = 0; i < n; i++){
		M[i] = (double*)malloc(sizeof(double)*n);
	}
	return M;
}

void leMatriz(double** M,int n){
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			scanf("%lf",&M[i][j]);
		}
	}
}

void imprimeMatriz(double** M,int n){
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n-1; j++){
			printf("%.1lf ",M[i][j]);
		}
		printf("%.1lf",M[i][n-1]);//sem espaço no fim da linha
		printf("\n");
	}
}

void freeMatriz(double** M,int n){
	for (int i = 0; i < n; i++){
		free(M[i]);
	}
	free(M);
}

int main(){
	int n;
	scanf("%d",&n);
	double **A, **B, **C, **D;
	A = alocaMatrizDouble(A,n);
	B = alocaMatrizDouble(B,n);
	C = alocaMatrizDouble(C,n);
	D = alocaMatrizDouble(D,n);

	leMatriz(A,n);
	leMatriz(B,n);
	leMatriz(C,n);
	leMatriz(D,n);

	//imprimeMatriz(A,n);
	//imprimeMatriz(B,n);
	//imprimeMatriz(C,n);
	//imprimeMatriz(D,n);
//_____________________________________________
	double** R1;
	R1 = alocaMatrizDouble(R1,n);

	double wtime = omp_get_wtime();

	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			for (int k = 0; k < n; k++){
				R1[i][j] += A[i][k]*B[k][j] + C[i][k]*D[k][j];
			}
		}
	}



	printf("%lf\n",omp_get_wtime() - wtime);

	//imprimeMatriz(R1,n);


	freeMatriz(R1,n);


	freeMatriz(A,n);
	freeMatriz(B,n);
	freeMatriz(C,n);
	freeMatriz(D,n);


}