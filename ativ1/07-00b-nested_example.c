#include <stdio.h>
#include <omp.h>

omp_nest_lock_t my_lock;
void Test(int i) {
   int tid = omp_get_thread_num( );
   omp_set_nest_lock(&my_lock);
   printf("Thread %d - starting nested locked region. i = %d \n", tid, i);
   printf("Thread %d - ending nested locked region. i = %d \n", tid, i);
   omp_unset_nest_lock(&my_lock);
}

int main() {
   omp_init_nest_lock(&my_lock);

   #pragma omp parallel num_threads(4)
   {
      int i, j;
      for (i = 0; i < 5; ++i) {
         	omp_set_nest_lock(&my_lock);
            if (i % 3)
               Test(i);
           omp_unset_nest_lock(&my_lock);
        }
    }
    omp_destroy_nest_lock(&my_lock);
}
