/*

*/

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define T 8


int main(int argc, char *argv[]) 
{
    int current = 0, retorno = 0;
    int tam;
    
    if ( argc  != 2)
    {
		printf("Wrong arguments. Please use binary <amount_of_elements>\n");
		exit(0);
    } // fim do if

    tam = atoi(argv[1]);

   
    #pragma omp parallel num_threads(T) firstprivate(current) shared(retorno)
    {
        #pragma omp single nowait
        {
            printf("Thread %d executando a região single para tamanho %d\n", omp_get_thread_num(), tam);
            while (current < tam)
            {
                #pragma omp task firstprivate(current) shared (retorno)
                {
                    printf("Thread %d executando a task %d \n", omp_get_thread_num(), current);
                    // se current é par, incrementa contador
                    if (current % 2 == 0)
                    {
                        #pragma omp critical (contapar)
                        retorno++;
                    }
                } // fim da task
                // incrementa valor local para mudar o valor para a próxima task
                current ++;
            } // fim do while
        } // fim da região do single
    } //fim da região paralela
    
    printf("Thread %d fora da região paralela; retorno %d \n", omp_get_thread_num(), retorno);
    return(0);
    
} // fim da main
