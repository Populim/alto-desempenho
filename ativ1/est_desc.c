#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// BEM VINDO !!!!
//Senha ssh: zG2JSf
double** cria_matriz(int n, int m) {
  double** mat = (double**) malloc(sizeof(double*) * n);
  if (mat == NULL) return NULL;
  for (int i = 0; i < n; i++) {
    mat[i] = (double*) malloc(sizeof(double) * m);
    if (mat == NULL) {
      for (i = i - 1; i >= 0; i--) free(mat[i]);
      free(mat);
      return NULL;
    }
  }
  return mat;
}

int cmp(const void* a, const void* b) {
  if (*(double*)a > *(double*)b)
    return 1;
  else if (*(double*)a < *(double*)b)
    return -1;
  else
    return 0;
}

double mediana(double* col, int tam_col) {
  if (tam_col % 2 != 0) {
    double res = col[(int)(tam_col / 2)];
    return res;
  } else {
    double res = (col[(tam_col / 2) - 1] + col[tam_col / 2]) / 2;
    return res;
  }
}

double moda(double* col, int tam_col) {
  int amodal = 1;

  int cnt_max = -1;
  double elem = -1;
  int cnt_temp = 1;
  for (int i = 0; i < tam_col - 1; i++) {
    if (col[i] == col[i + 1])
      cnt_temp++;
    else {
      if(cnt_max != -1 && cnt_temp != cnt_max )
        amodal = 0;
      if (cnt_temp > cnt_max) {
        cnt_max = cnt_temp;
        elem = col[i];
      }
      cnt_temp = 1;
    }
  }
  if (cnt_temp > cnt_max) {  // verifica ultima pos
    cnt_max = cnt_temp;
    elem = col[tam_col - 1];
  }
  return amodal ? -1 : elem;
}

double media_aritmetica(double* col, int tam_col) {
  double media = 0;
  for (int i = 0; i < tam_col; i++) {
    media += col[i];
    
  }
  media /= tam_col;
  return media;
}

double media_harmonica(double* col, int tam_col) {
  double media = 0;
  for (int i = 0; i < tam_col; i++) {
    media += 1 / col[i];
  }
  return tam_col / media;
}

double variancia(double* col, int tam_col, double media) {
  double var = 0;
  for (int i = 0; i < tam_col; i++) {
    var += (col[i] - media) * (col[i] - media);
  }
  var /= (tam_col - 1);  
  return var;
}

double desvio_padrao(double var) { return sqrt(var); }

double coef_variacao(double desv, double media) { return desv / media; }

void print_result(double* mediaA, double* mediaH, double* medianas,
                  double* modas, double* desvios, double* variancias,
                  double* coeficientes, int m){
  for(int i=0;i<m;i++)
    printf("%.1lf ", mediaA[i]);
  printf("\n");
  
  for(int i=0;i<m;i++)
    printf("%.1lf ", mediaH[i]);
  printf("\n");

  for(int i=0;i<m;i++)
    printf("%.1lf ", medianas[i]);
  printf("\n");
  
  for(int i=0;i<m;i++)
    printf("%.1lf ", modas[i]);
  printf("\n");

  for(int i=0;i<m;i++)
    printf("%.1lf ", variancias[i]);
  printf("\n");
  
  for(int i=0;i<m;i++)
    printf("%.1lf ", desvios[i]);
  printf("\n");

  for(int i=0;i<m;i++)
    printf("%.1lf ", coeficientes[i]);
  printf("\n");
}

void print_matriz(double **mat, int n,int m){
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      printf("%lf ", mat[i][j]);
    }
    printf("\n");
  }
}

int main(int argc, char const* argv[]) {
  int n, m;


  scanf("%d %d", &n, &m);
  
  double* mediaA = malloc(sizeof(double)*m);
  double* mediaH = malloc(sizeof(double)*m);
  double* medianas = malloc(sizeof(double)*m);
  double* modas = malloc(sizeof(double)*m);
  double* desvios = malloc(sizeof(double)*m);
  double* variancias = malloc(sizeof(double)*m);
  double* coeficientes = malloc(sizeof(double)*m);
  
  double wtime;
  double** mat = cria_matriz(m, n);
  double** cpy = cria_matriz(m, n);
  if (mat == NULL || cpy == NULL) {
    printf("SEM MEMORIA PARA ALOCAR MATRIZ!!\n");
    return 0;
  }

  // Leitura da matriz transposta para facilitar o uso de colunas como
  // parâmetros
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      scanf("%lf", &mat[j][i]);
      cpy[j][i] = mat[j][i];
    }
  }
  
  int i=0;
  #pragma omp parallel shared(mat,cpy,mediaA,mediaH,medianas,modas,variancias,desvios,coeficientes) 
  {
    #pragma omp for private(i)
    for(i=0; i<m;i++){
      //SEM SINGLE POIS SO UMA THREAD ENTRA EM CADA ITERAÇÃO DO FOR
      #pragma omp task depend(out: mediaA[i])//media aritimetica
      { 
        mediaA[i] = media_aritmetica(mat[i],n);
      }
      
      #pragma omp task //media harmonica
      {
        mediaH[i] = media_harmonica(mat[i], n);
      }
      
      #pragma omp task depend(out: cpy[i])// qsort
      {
        qsort(cpy[i], n, sizeof(double), cmp);
      }

      //depende qsort
      #pragma omp task depend(in: cpy[i]) //mediana
      {
        medianas[i] = mediana(cpy[i],n);
      }
      
      //depende qsort
      #pragma omp task depend(in: cpy[i]) //moda
      {
        modas[i] = moda(cpy[i], n);
      }
      //depende media
      #pragma omp task depend(in: mediaA[i]) depend(out:variancias[i]) //variancia
      {
        variancias[i] = variancia(mat[i], n, mediaA[i]);
      }
      //depende variancia
      #pragma omp task depend(in:variancias[i]) depend(out:desvios[i])//desvio padrao
      {
        desvios[i] = desvio_padrao(variancias[i]);
      }
      //depende desvio e media
      #pragma omp task depend(in:desvios[i], mediaA[i]) //coeficiente de variação
      {
        coeficientes[i] = coef_variacao(desvios[i], mediaA[i]);
      }
    }
  }

  print_result(mediaA, mediaH, medianas, modas, desvios, variancias,
              coeficientes, m);

    for(int i=0;i<m;i++)
    for(int j=0;j<n;j++)
      cpy[i][j] = mat[i][j];

  //SEQUENCIAL-------------------------
  clock_t start = clock();
  for(int i=0;i<m;i++){
    mediaA[i] = media_aritmetica(mat[i],n);
    mediaH[i] = media_harmonica(mat[i],n);
    variancias[i] = variancia(mat[i], n, mediaA[i]);
    desvios[i] = desvio_padrao(variancias[i]);
    qsort(cpy[i], n, sizeof(double), cmp);
    medianas[i] = mediana(cpy[i],n);
    modas[i] = moda(cpy[i], n);
    coeficientes[i] = coef_variacao(desvios[i], mediaA[i]);

  }
  clock_t end = clock();
  //---------------------------------------
  printf("\n\n");
  print_result(mediaA, mediaH, medianas, modas, desvios, variancias,
               coeficientes, m);

  for (int i = 0; i < m; i++) free(mat[i]);
  free(mat);

  for (int i = 0; i < m; i++) free(cpy[i]);
  free(cpy);


  free(mediaA);
  free(mediaH);
  free(modas);
  free(medianas);
  free(desvios);
  free(variancias);
  free(coeficientes);
  return 0;
}
