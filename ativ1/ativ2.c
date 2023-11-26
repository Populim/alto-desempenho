// compilar: make all
// executar: make run (para o arquivo entrada.txt)
// ou: ./est_desc < entrada.txt

// Alunos:
// Bruno Baldissera
// Bruno Gazoni
// Gabriel Eluan Calado
// Matheus Steigenberg Populim
// João Nascimento Villaça


#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>
#define T 8

//Função necessária para o uso do qsort. Define a comparação usual entre tipos double.
int compare (const void * a, const void * b){
	if ( *(double*)a <  *(double*)b ) return -1;
	else if ( *(double*)a == *(double*)b ) return 0;
	else if ( *(double*)a >  *(double*)b ) return 1;
}

//O cálculo da média aritmética é feito paralelizando um loop com o somatório de todos os elementos,
// onde na reduction todas as somas parciais das threads são compiladas em uma soma global, que então é dividida
// pelo tamanho n e retornada.
double media_aritmetica(double* v, int n){
	double total = 0;
	#pragma omp parallel for num_threads(T) reduction(+:total)
	for (int i = 0; i < n; i++){
		total += v[i];
	}
	total /= n;
	return total;
}

//Esta computação é feita de forma similar à média aritmética, mas cada parcela das somas é o inverso do elemento
// de uma linha da matriz, e a última operação é feita dividindo o tamanho n pela soma global após a redução.
double media_harmonica(double* v, int n){
	double total = 0;
	#pragma omp parallel for num_threads(T) reduction(+:total)
	for (int i = 0; i < n; i++){
		total += 1/v[i];
	}
	total = n/total;
	return total;
}

//Para essa função funcionar, o vetor v tem que estar ordenado.
//Se o vetor tem tamanho par, retornamos a média dos dois elementos centrais,
// senão, retornamos o valor do elemento central.
double getmediana(double* v, int n){
	if(n%2 == 0){
		return (v[n/2 - 1]+v[n/2])/2.0;
	}
	else{
		return v[n/2];
	}
}

//Para essa função funcionar, o vetor v tem que estar ordenado.
//Aqui usamos uma computação sequencial, onde criamos uma estrutura de bucket que serve para contar
// quantas vezes cada valor do vetor se repete. Com uma variável auxiliar que guarda número que mais se repete
// dentre os elementos do bucket, conseguimos guardar a moda, de forma que se só a atualizamos se houver um elemento contado
// mais vezes do que ela. Para a condição de amodalidade, verificamos se a quantidade de números contados diferentes vezes é
// igual ao tamanho do vetor (só acontece se todos os elementos aparecem exatamente uma vez).
double getmoda(double* v, int n){
	double vv[n];
	int vvcount[n];
	int j = 0;
	for (int i = 0; i < n; i++){
		vvcount[i] = 0;
	}
	vv[0] = v[0];
	for (int i = 0; i < n; i++){
		if(v[i] == vv[j]){
			vvcount[j]++;
		}
		else{
			++j;
			vv[j] = v[i];
			vvcount[j]++;
		}
	}
	j++;
	int maior = vvcount[0];
	int thebiggest = vv[0];
	int flag = 0;
	int sem_moda = 1;
	int igual = vvcount[0];
	for (int k = 0; k < j; k++){
		if(igual != vvcount[k]){
			sem_moda = 0;
			break;
		}
	}
	if(sem_moda == 1){
		return -1;
	}
	for (int k = 0; k < j; k++){
		if(maior < vvcount[k]){
			maior = vvcount[k];
			thebiggest = vv[k];
			flag = 1;
		}
	}
	return thebiggest;
}

//Esta computação sequencial utiliza a média, e a cada elemento subtrai a média deste e eleva ao quadrado esta diferença.
// Ao final o somatório destes termos é dividido por (n-1) e retornado.
double variation(double* v,double m, int n){
	double total = 0;
	double aux = 0;
	for (int i = 0; i < n; i++){
		aux = v[i]-m;
		total += aux*aux;
	}
	total = total/(n-1);
	return total;
}

//Simplesmente recebemos como parâmetro a variância e retornamos sua raiz quadrada.
double getstd(double var){
	return sqrt(var);
}


int main(int argc,char **argv){
	int a,n;
	scanf("%d %d", &n, &a);

	//São declarados e alocados aqui os vetores que  guardam os dados estatísticos posicionados corretamente.
	double* media = (double*)malloc(sizeof(double)*a);
	double* media_h = (double*)malloc(sizeof(double)*a);
	double* moda = (double*)malloc(sizeof(double)*a);
	double* mediana = (double*)malloc(sizeof(double)*a);
	double* var = (double*)malloc(sizeof(double)*a);
	double* std = (double*)malloc(sizeof(double)*a);
	double* cv = (double*)malloc(sizeof(double)*a);
	double** mat = (double**)malloc(sizeof(double*)*a);

	//Alocamos a matriz.
	for (int i = 0; i < a; i++){
		mat[i] = (double*)malloc(sizeof(double)*n);	
	}

	//Lemos os inputs para as posições da matriz.
	for (int i = 0; i < n; i++){
		for (int j = 0; j < a; j++){
			scanf("%lf", &mat[j][i]);
		}
	}

	double wtime = omp_get_wtime();
	double wtime2;
	int my_id;
	int i = 0;

	//Criamos a região paralela principal via um omp parallel for, que já delega a cada thread uma porção de cada região
	// a ser percorrida no loop, tendo como variáveis compartilhadas todos os dados estatísticos, e como privado apenas o índice i.
	//Separamos as computações em tasks com dependências compatíveis, por exemplo, deixando todas as funções que precisam de vetores ordenados juntas,
	// após a ordenação destes, e fazendo os cálculos de média, variância, desvio padrão e variância juntos, pois um depende do retorno do outro em cascata.
	#pragma omp parallel for num_threads(T) shared(mat,media_h,media, moda, mediana, var, std,cv) private(i)
		for (i = 0; i < a; i++){
			#pragma omp task
			{
				media_h[i] = media_harmonica(mat[i],n);
			}
			#pragma omp task
			{
				media[i] = media_aritmetica(mat[i],n);
				var[i] = variation(mat[i],media[i],n);
				std[i] = getstd(var[i]);
				cv[i] = std[i]/media[i];
			}
			#pragma omp task
			{
				qsort(mat[i],n,sizeof(double),compare);

				#pragma omp task
				{	
					mediana[i] = getmediana(mat[i],n);
				}
				#pragma omp task
				{	
					moda[i] = getmoda(mat[i],n);
				}
			}
		}


	for (int i = 0; i < a; i++){
		printf("Mean: %.1lf M_h: %.1lf mode: %.1lf median: %.1lf var: %.1lf std: %.1lf cv: %.1lf\n", media[i],media_h[i], moda[i], mediana[i], var[i], std[i],cv[i]);
	}

	printf("\n");
	wtime = omp_get_wtime() - wtime;
	printf("tempo total: %g milisegundos\n", wtime*1000.0);
	//printf("time quick sort: %g\n", wtime2*1000.0);
	for (int i = 0; i < a; i++){
		free(mat[i]);
	}
	free(mat);

	free(media);
	free(media_h);
	free(mediana);
	free(moda);
	free(var);
	free(std);
	free(cv);
}