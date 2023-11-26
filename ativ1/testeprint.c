#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>
#define T 8

int funcaodocaralho(int a){
	for (int i = 0; i < a; i++){
		for (int i = 0; i < a; i++){
			for (int i = 0; i < a; i++){
				i++;
			}
		}
	}
	return a;
}


int main(){
	int a  = 20;
	int teste = 0;

	#pragma parallel for num_threads(T) private(teste)
	for (int i = 0; i < a; i++){
		#pragma omp task shared(teste)
		{
			if (teste == 0){
				teste = 1;
				teste = funcaodocaralho(teste);
			}
			//printf("i want it that way\n");

		}
		#pragma omp task shared(teste)
		{
			if (teste == 1){
				teste = 2;
				teste = funcaodocaralho(teste);
			}
			else if(teste == 0){
				teste = 3;
				teste = funcaodocaralho(teste);
			}
			//printf("i want it that way\n");

		}
		#pragma omp task shared(teste)
		{
			if (teste == 0){
				teste = 5;
				teste = funcaodocaralho(teste);
			}
			else if (teste == 1){
				teste = 4;
				teste = funcaodocaralho(teste);
			}
			else if (teste == 2){
				teste = 7;
				teste = funcaodocaralho(teste);
			}
			else if (teste == 3){
				teste = 9;
				teste = funcaodocaralho(teste);
			}
			//printf("i want it that way\n");
		}
		printf("%d\n", teste);
	}

}


