#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>
#define T 2


int compare (const void * a, const void * b){
	if ( *(double*)a <  *(double*)b ) return -1;
	else if ( *(double*)a == *(double*)b ) return 0;
	else if ( *(double*)a >  *(double*)b ) return 1;
}

double media_aritmetica(double* v, int n){
	double total = 0;
	#pragma omp parallel for num_threads(T) reduction(+:total)
	for (int i = 0; i < n; i++){
		total += v[i];
	}
	total /= n;
	return total;
}

double media_harmonica(double* v, int n){
	double total = 0;
	#pragma omp parallel for num_threads(T) reduction(+:total)
	for (int i = 0; i < n; i++){
		total += 1/v[i];
	}
	total = n/total;
	return total;
}

//para essa função funcionar, o vetor v tem que estar ordenado
double getmediana(double* v, int n){
	if(n%2 == 0){
		return (v[n/2 - 1]+v[n/2])/2.0;
	}
	else{
		return v[n/2];
	}
}

//para essa função funcionar, o vetor v tem que estar ordenado
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

double getstd(double var){
	return sqrt(var);
}


int main(int argc,char **argv){
	int a,n;
	scanf("%d %d", &n, &a);
	double* media = (double*)malloc(sizeof(double)*a);
	double* media_h = (double*)malloc(sizeof(double)*a);
	double* moda = (double*)malloc(sizeof(double)*a);
	double* mediana = (double*)malloc(sizeof(double)*a);
	double* var = (double*)malloc(sizeof(double)*a);
	double* std = (double*)malloc(sizeof(double)*a);
	double* cv = (double*)malloc(sizeof(double)*a);
	double** mat = (double**)malloc(sizeof(double*)*a);
	for (int i = 0; i < a; i++){
		mat[i] = (double*)malloc(sizeof(double)*n);	
	}

	double** mat1 = (double**)malloc(sizeof(double*)*a);
	for (int i = 0; i < a; i++){
		mat1[i] = (double*)malloc(sizeof(double)*n);	
	}

	for (int i = 0; i < n; i++){
		for (int j = 0; j < a; j++){
			scanf("%lf", &mat[j][i]);
			mat1[j][i] = mat[j][i];////////////
		}
	}

	double wtime = omp_get_wtime();
	int my_id;
	int i = 0;
	#pragma omp parallel for num_threads(T) shared(mat,media_h,media, moda, mediana, var, std,cv)
		for (i = 0; i < a; i++){
			media_h[i] = media_harmonica(mat[i],n);
			media[i] = media_aritmetica(mat[i],n);
			var[i] = variation(mat[i],media[i],n);
			std[i] = getstd(var[i]);
			cv[i] = std[i]/media[i];
			qsort(mat[i],n,sizeof(double),compare);
			mediana[i] = getmediana(mat[i],n);
			moda[i] = getmoda(mat[i],n);
		}

	wtime = omp_get_wtime() - wtime;
	printf("time total: %g\n", wtime*1000);

	for (int i = 0; i < a; i++){
		//printf("Mean: %g M_h: %g mode: %g median: %g var: %g std: %g cv: %g\n", media[i],media_h[i], moda[i], mediana[i], var[i], std[i],cv[i]);
	}

	wtime = omp_get_wtime();
	for (i = 0; i < a; i++){
		media_h[i] = media_harmonica(mat1[i],n);
		media[i] = media_aritmetica(mat1[i],n);
		var[i] = variation(mat1[i],media[i],n);
		std[i] = getstd(var[i]);
		cv[i] = std[i]/media[i];
		qsort(mat1[i],n,sizeof(double),compare);
		mediana[i] = getmediana(mat1[i],n);
		moda[i] = getmoda(mat1[i],n);
	}
	wtime = omp_get_wtime() - wtime;
	printf("time total sequencial: %g\n", wtime*1000);

	for (int i = 0; i < 50; i++){
		//printf("Mean: %g M_h: %g mode: %g median: %g var: %g std: %g cv: %g\n", media[i],media_h[i], moda[i], mediana[i], var[i], std[i],cv[i]);
	}

	printf("\n");
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