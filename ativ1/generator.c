#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc,char **argv){
	int a,n;
	//printf("digite o número de linhas (conjuntos de dados)\ne o número de dados por conjunto\n");
	scanf("%d %d",&a,&n);

	double vet[n];

	for (int i = 0; i < n; i++){
		vet[i] = (rand()%100+10)/10.0;
	}
	printf("%d %d\n", n,a);
	for (int i = 0; i < n; i++){
		for (int j = 0; j < a; j++){
			printf("%g ", vet[i]);
		}
		printf("\n");
	}
}