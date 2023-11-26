#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define NUM_SPAWNS 8

int main( int argc, char *argv[] )
{
	int np = NUM_SPAWNS;
	int errcodes[NUM_SPAWNS];
	MPI_Comm parentcomm, intercomm;

	MPI_Init( &argc, &argv );
	MPI_Comm_get_parent( &parentcomm );
	if (parentcomm == MPI_COMM_NULL) {
		MPI_Comm_spawn( "stack", MPI_ARGV_NULL, np-1, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &intercomm, errcodes );
		printf("I'm the parent.\n");
	}
	else {
		printf("I'm the spawned.\n");
	}
	fflush(stdout);
	MPI_Finalize();
	return 0;
}