// para compilar: mpicc vet.c -o vet -Wall
// para rodar (use uma delas): 
// mpirun -np 4 vet   (supondo 04 cores)
// mpirun --use-hwthread-cpus -np 8 vet (supondo 4 cores e 2 threads por core)
// mpirun --oversubscribe -np 10 vet (sobreposicao)
// mpirun -np 8 --hostfile hosts vet (segue o hostfile e deve obedecer limitacoes)
// mpirun -np 8 --host hostname:slots vet (segue -host e segue limitacoes. slots é opcional (default 1))
// mpirun --map-by node --hostfile ./halley.txt -np 4 ./vet : -np 4 ./vet  (espalha processos em nós)
// mpirun --map-by core --hostfile ./halley.txt -np 4 ./vet : -np 4 ./vet (espalha processos em cores)
//
//
// GRUPO 06 TURMA A:
// Alunos:
// Bruno Baldissera Carlotto 	10724351
// Bruno Gazoni		           	7585037
// Gabriel Eluan Calado		   	10734453
// João Villaça 			   	10724239
// Matheus Steigenberg Populim  10734710

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char *argv[])  {
	int start,end,tam,n,k,np,rank,ret,limiar,count;
	int* vet,*maiores;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	//nome do processador
	char pname[MPI_MAX_PROCESSOR_NAME];	
	int name_len;
	MPI_Get_processor_name(pname, &name_len);

	//printf("I am process %d from processor %s.\n", rank, pname);
	
	//caso seja o processo master, nesse caso o proc com rank == 0 , lê a entrada padrão,
	//faz o particionamento do vetor para que cada processo procure os
	//elementos maiores que o limiar em suas respectivas partições,
	//recebe os elementos e os imprime:
	if(rank == 0){
		//lê os dados da entrada padrão:
		scanf("%d",&n);
		scanf("%d",&k);
		vet = (int*) malloc(n*sizeof(int));
		for (int i = 0; i < n; i++)
			scanf("%d",&vet[i]);
		limiar = vet[k];

		for (int i = 1; i < np; i++){
			//manda o tamanho total do vet
			MPI_Send(&n,1,MPI_INT,i,i,MPI_COMM_WORLD);	
			//manda o valor do elemento a ser comparado
			MPI_Send(&(limiar),1,MPI_INT,i,i,MPI_COMM_WORLD);
			
			//delimita os limites do particionamento do vetor
			start = (i*n)/np;
			end =  (n*(i+1))/np;
			//envia a posição inicial do vetor original,
			//para que se possa determinar a posição de um elemento
			//de um vetor particionado no vetor original
			MPI_Send(&(start),1,MPI_INT,i,i,MPI_COMM_WORLD);

			//manda uma partição do vetor para o processo i
			//printf("Proc %s de rank %d enviou %d nros para o proc %d\n",pname,rank,end-start,i);
			MPI_Send(&(vet[start]),end-start,MPI_INT,i,i,MPI_COMM_WORLD);
		}

		//calcula o tamanho da primeira partição
		tam = (n*(rank+1))/np - (rank*n)/np;
		//aloca um vetor para armazenar a posição dos
		//elementos maiores que o limiar
		int* maiores = (int*) malloc(sizeof(int)*n);
		count = 0;//número de elementos maiores que o limiar
		int countaux = 0;
		for (int i = 0; i < tam; i++){
			if(vet[i] > limiar){
				maiores[count++] = i;//verifica os maiores elementos na partição 0
			}
		}
		for (int i = 1; i < np; i++){
			//recebe o número de elementos maiores que o limiar na partição i
			MPI_Recv(&countaux,1,MPI_INT,i,i,MPI_COMM_WORLD,&status);
			
			//recebe a posição dos elementos maiores que o limiar na partição i
			//estes elementos são inserido a partir da última posição
			//ocupada no vetor "maiores"
			MPI_Recv(&(maiores[count]),countaux,MPI_INT,i,i,MPI_COMM_WORLD,&status);
			count+=countaux;//atualiza o tamanho do vetor "maiores"
		}
		//imprime o nro de números maiores que o limiar
		printf("%d \n", count);
		//imprime as posições
		for (int i = 0; i < count; i++){
			printf("%d ", maiores[i]);
		}
		printf("\n");

	}
	// caso o processo seja um processo slave, ou seja, um proc com rank != 0,
	// recebe a partição do vetor original, assim como o tamanho n
	// necessário para calcular o tamanho da partição, o limiar e a
	// posição inicial da partição.
	// Após isso, verifica a posição dos elementos maiores que o limiar,
	// e retorna as posições juntamente com a quantidade para o processo
	// master.
	else{
		//recebe o tamanho total do vetor, a fim de calcular o tamanho da partição
		MPI_Recv(&n,1,MPI_INT,0,rank,MPI_COMM_WORLD,&status);
		//calcula o tamanho da partição
		tam = (n*(rank+1))/np - (rank*n)/np;
		
		//recebe o limiar e a posição de início da partição em relação
		//ao vetor original:
		MPI_Recv(&limiar,1,MPI_INT,0,rank,MPI_COMM_WORLD,&status);
		MPI_Recv(&start,1,MPI_INT,0,rank,MPI_COMM_WORLD,&status);
		
		//aloca o vetor que receberá a partição, o vetor "maiores" desta partição
		// e recebe os números:
		vet = (int*) malloc(sizeof(int)*(tam));
		maiores = (int*) malloc(sizeof(int)*(tam));
		MPI_Recv(vet,tam,MPI_INT,0,rank,MPI_COMM_WORLD,&status);
		
		//contador do número de elementos maiores que o limiar nesta partição
		count = 0;

		//verifica quais elementos são maiores que o limiar
		for (int i = 0; i < tam; i++){
			if(vet[i]> limiar){
				maiores[count++] = i + start;//adiciona a posição inicial da partição
			}
		}
		//retorna para o processo 0 o número de elementos maiores, assim como
		// as posições dos elementos em si:
		MPI_Send(&count,1,MPI_INT,0,rank,MPI_COMM_WORLD);
		MPI_Send(maiores,count,MPI_INT,0,rank,MPI_COMM_WORLD);
	}

	//libera os vetores e encerra o programa:
	free(maiores);
	free(vet);
	ret = MPI_Finalize();
	if (ret == MPI_SUCCESS){
		//printf("MPI_Finalize success! \n");
	}
	return 0;
}
