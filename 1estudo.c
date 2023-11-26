#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

void* sort(void* a){
	int n = 20;
	int vetor[n];
	int swap;
	printf("\n");

	for (int i = 0; i < n; ++i){
		vetor[i] = rand()%10;
	}
	for (int i = 0; i < 20; ++i){
		printf("%d ", vetor[i]);
	}
	printf("\n");
    for (int i = 1; i < n; i++) {
        swap = vetor[i];
        int j = i - 1;
        while (j >= 0 && vetor[j] > swap) {
            vetor[j + 1] = vetor[j];
            j = j - 1;
        }
        vetor[j + 1] = swap;
    }
	for (int i = 0; i < 20; ++i){
		printf("%d ", vetor[i]);
	}
	printf("\n");
	pthread_exit(NULL);	
}

//mergesort ->
//arguments: start,middle,end
void mergesort(int*v, int s,int m,int e){
	if(s == e){
		return;
	}
	else{
		int middle = ((int)(s+e))/2;
	}
}

int main(){


	//fazer merge sort
	int N = 10;
	int* v = malloc(sizeof(int)*N);
	for (int i = 0; i < N; ++i){
		v[i] = rand();
	}
	for (int i = 0; i < N; ++i){
		printf("%d  ",v[i]);
		/* code */
	}
	printf("\n");




	/*
	int N = 8;
	pthread_t threads[N];

	int ret;
	for (int i = 0; i < N; i++){
		ret = pthread_create(&threads[i],NULL, sort,&i);
		//pthread_join(threads[i],NULL);
	}
	// APENAS DAR JOIN APÓS CRIAR TODAS AS THREADS

	for (int i = 0; i < N; i++){
		//ret = pthread_create(&threads[i],NULL, sort,&i);
		pthread_join(threads[i],NULL);
	}
	*/

}