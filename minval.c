//main program 
#include "minval.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>
int main(int argc, char* argv[]) {
  // Initialize MPI
  int rank, num_procs;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  double t1,t2; // time 
  // Check that there are exactly two processes
  if (num_procs != 2) {// only two mpi proccess 
     fprintf(stderr, "Error: this program must be run with exactly two processes.\n");
     MPI_Abort(MPI_COMM_WORLD, 1);
  }

  // Read in the values from "input.txt"
  int n;
  double* a;
    t1=MPI_Wtime(); // starter timer
    first_proccess_read(&n, &a, rank); 
    
    // Divide the array into two halves
    int n1 = n / 2;
    int n2 = n - n1;
    double* a1 = a;
    double* a2 = a + n1;

    // Allocate memory for the second half of the array (if necessary)
    if (rank == 1) {
    a2 = malloc(n2 * sizeof(double));
    if (!a2) {
    MPI_Abort(MPI_COMM_WORLD, 1);
    error("Error allocating memory for array A2");
    }
    }
    // Send the second half
    partition_in_half(a, &a2, n2, rank);

    // Calculate the minimal value of array B using OpenMP
    int num_threads = omp_get_max_threads();
    double min_val = INFINITY;
    min_val = get_min(a2, n2, num_threads);

    // Reduce the minimal values calculated by each process
    double global_min;
    global_min = find_final_min(min_val, rank);

    // Print the global minimal value
    if (rank == 0) {
    t2=MPI_Wtime(); // end timer
    printf("minimal value of array B is: %lf\n", global_min);
    printf("Execution time: %lf\n",t2-t1);
    }

    // Free memory and finalize MPI
    free(a);
    if (rank == 1) {
    free(a2);
    }
    MPI_Finalize();
    return 0;
    }
