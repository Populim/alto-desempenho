#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {

  MPI_Init(NULL, NULL);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  int *nums;
  nums=(int*)malloc(world_size*(sizeof(int)));

  float local_sum = 0;
  int i;
  for (i = 0; i < world_size; i++) {
    nums[i]=i*world_rank;
    local_sum += nums[i];
  }
  printf("Local sum for process %d - %f\n",world_rank, local_sum);

  // Reduce all of the local sums into the global sum
  float global_sum;
  MPI_Scan(&local_sum, &global_sum, 1, MPI_FLOAT, MPI_SUM,MPI_COMM_WORLD);


  // Clean up
  free(nums);

  MPI_Barrier(MPI_COMM_WORLD);

  // Print the results
  printf("Process %d has Local sum = %f, Global sum = %f\n", world_rank, local_sum, global_sum);
  MPI_Finalize();
}
