#include <stdio.h>
#include <omp.h>
// import entradaTeclado

int main(){
	int n;
	scanf("%d\n",&n);
	fflush(0);
	char string[n+1];
	fgets(string,n+1,stdin);
	//scanf("%[^\n]",string);
	//printf("%s\n", string);

	int bucket[256] = {0};
	//aabbbccccaaaa
	for (int i = 0; i < n; i++){
		bucket[string[i]]++;
	}

	int maior = bucket[0];//nro de vezes que o char mais frequente aparece
	for (int j = 0; j < 256; j++){
		if(maior < bucket[j]){
			maior = bucket[j];
		}
	}


	char resultado[256] = {0};
	int index = 0;
	for (int j = 0; j < 256; j++){
		if(bucket[j] == maior){
			resultado[index] = j;
			index++;
		}
	}
	// 1 2 3 3 3 3 3 3 4 4 4 4 4 4 5//


	for (int i = 0; i < index; i++){
		printf("'%c' se repete %d vezes.\n", resultado[i],maior);	
	}


}