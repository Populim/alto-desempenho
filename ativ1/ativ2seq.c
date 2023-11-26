#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>
#define T 8


int compare (const void * a, const void * b){
	if ( *(double*)a <  *(double*)b ) return -1;
	else if ( *(double*)a == *(double*)b ) return 0;
	else if ( *(double*)a >  *(double*)b ) return 1;
}

double media_aritmetica(double* v, int n){
	double total = 0;
	//#pragma parallel for num_threads(T) reduction(+:sum)
	for (int i = 0; i < n; i++){
		total += v[i];
	}
	total /= n;
	return total;
}

double media_harmonica(double* v, int n){
	double total = 0;
	//#pragma parallel for num_threads(T) reduction(+:sum)
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
	int maior = vvcount[j-1];
	int thebiggest = 0;
	int flag = 0;
	int flag2 = 0;
	for (int k = j-1; k >= 0; k--){
		//printf("%d ", vvcount[k]);
		if(maior != vvcount[k]){
			flag2 = 1;
		}
		if(maior <= vvcount[k]){
			maior = vvcount[k];
			thebiggest = vv[k];
			flag = 1;
		}
	}
	if(flag == 0 || flag2 == 0){
		return -1;
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
	double media, media_h, moda, mediana, var, std,cv;
	double** mat = malloc(sizeof(double*)*a);
	for (int i = 0; i < a; i++){
		mat[i] = malloc(sizeof(double)*n);	
	}

	for (int i = 0; i < n; i++){
		for (int j = 0; j < a; j++){
			scanf("%lf", &mat[j][i]);
		}
	}

	/*

	****************** T1
	****************** T2
	****************** T3
	****************** T4
	******************
	******************
	******************
	******************
	*/


	double wtime = omp_get_wtime();
	double wtime2;

	for (int i = 0; i < a; i++){

			media_h = media_harmonica(mat[i],n);

			media = media_aritmetica(mat[i],n);

			var = variation(mat[i],media,n);

			std = getstd(var);

			cv = std/media;

			wtime2 = omp_get_wtime();
			qsort(mat[i],n,sizeof(double),compare);
			wtime2 = omp_get_wtime() - wtime2;

			mediana = getmediana(mat[i],n);



			moda = getmoda(mat[i],n);

		//printf("media:%g harm %g moda:%g mediana:%g var:%g std:%g cv:%g\n", media, media_h,moda,mediana,var,std,cv);
	}
	wtime = omp_get_wtime() - wtime;
	printf("time total: %g\n", wtime*1000.0);
	printf("time quick sort: %g\n", wtime2*1000.0);
	for (int i = 0; i < a; i++){
		free(mat[i]);
	}
	free(mat);
}