#include <stdio.h>


int main(){
	int np = 8;
	int n = 47;
	int start,end;
	int soma = 0;
	for (int i = 0; i < np; i++){
		start = (i*n)/np;
		end = ((i+1)*n)/np;
		printf("%d %d %d\n",start,end,end-start);
		soma+=n/np;
	}
	printf("Soma: %d\n", soma);
}