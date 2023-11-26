// compilar: make all
// executar: make run (para o arquivo entrada.txt)
// ou: ./freq_char < entrada.txt

// Alunos:
// Bruno Baldissera
// Bruno Gazoni
// Gabriel Eluan Calado
// Matheus Steigenberg Populim
// João Nascimento Villaça

#include <stdio.h>
#include <omp.h>
#define T 8

int main(){
	int n, i;
	int index = 0;
	char resultado[256] = {0};
	int bucket[256] = {0};
	scanf("%d\n",&n);
	char string[n+1];
	fflush(0);
	fgets(string,n+1,stdin);

	#pragma omp parallel for num_threads(T) private(i) shared(bucket)
	for (i = 0; i < n; i++){
		#pragma omp critical //critical na região onde há leitura e escrita da
		{ 					// variável compartilhada	  
			bucket[string[i]]++;
		}
	}
	
	int maior = bucket[0];//nro de vezes que o char mais frequente aparece
	for (i = 0; i < 256; i++){
		if(maior < bucket[i]){
			maior = bucket[i];
		}
	}
	//verifica se há outros caracteres com a mesma frequência
	for (i = 0; i < 256; i++){
		if(bucket[i] == maior){
			resultado[index] = i;
			index++;
		}
	}
	for (i = 0; i < index; i++){
		printf("%c\n", resultado[i]);	
	}
}