// para compilar: mpicc token.c -o token -Wall
// para rodar (use uma delas): 
// mpirun -np 4 token   (supondo 04 cores)
// mpirun --use-hwthread-cpus -np 8 token (supondo 4 cores e 2 threads por core)
// mpirun --oversubscribe -np 10 token (sobreposicao)
// mpirun -np 8 --hostfile hosts token (segue o hostfile e deve obedecer limitacoes)
// mpirun -np 8 --host hostname:slots token (segue -host e segue limitacoes. slots é opcional (default 1))
// mpirun --map-by node --hostfile ./halley.txt -np 4 ./token : -np 4 ./token  (espalha processos em nós)
// mpirun --map-by core --hostfile ./halley.txt -np 4 ./token : -np 4 ./token (espalha processos em cores)
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
	int np, rank, ret,token;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);

	if ( rank == 0) {
		token = rank;

		//esses prints demonstram que realmente processos diferentes
		//estão sendo usados para executar o programa
		//printf("There are %d processes.\n", np);
		//printf("I am process %d from processor %s\n", rank, processor_name);

		//envia o token para o proc 1
		MPI_Send(&token, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);

		//recebe o último token do proc np-1
		MPI_Recv(&token, 1, MPI_INT, np-1, 0, MPI_COMM_WORLD, &status);

		printf("\nA jornada do anel foi concluída:\nRecebido o token %d no processador %d\n", token,rank);
	}
	else {
		//printf("Eu sou o processo %d do proc %s\n", rank, processor_name);
		//recebe o token do proc anterior
		MPI_Recv(&token, 1, MPI_INT, rank-1, rank, MPI_COMM_WORLD, &status);
		token++;//incrementa o token
		//caso o proc seja o último, envia o token para o inicial:
		if(rank == np-1){
			MPI_Send(&token, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
		else{
			MPI_Send(&token, 1, MPI_INT, rank+1, rank+1, MPI_COMM_WORLD);
		}
		//printf("acabei aqui no processador %d\n", rank);
	}
	fflush(0);
	ret = MPI_Finalize();
	if (ret == MPI_SUCCESS){
		//printf("MPI_Finalize success! \n");
	}
	return(0);
}
