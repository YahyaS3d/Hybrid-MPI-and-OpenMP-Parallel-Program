//functions declaration  
#include "minval.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>

void error(char* str){
  perror(str);
  MPI_Abort(MPI_COMM_WORLD, 1);
}
void first_proccess_read(int* N, double** A, int rank) {
double t1;
if (rank == 0) {
// Open the input file
FILE* input_file = fopen(FILE_NAME, "r");
if (!input_file) {
error("Error opening the file");
}

// Read the first line of the file (containing N)
if (fscanf(input_file, "%d", N) != 1) {
  error("Error reading first line from input file");// error reading N 
}

// Allocate memory for the array
*A = malloc(*N * sizeof(double));
if (!*A) {
  error("allocating memory for array A failed");
}

// Read the values from the file
for (int i = 0; i < *N; i++) {
  if (fscanf(input_file, "%lf", &(*A)[i]) != 1) {
      error("Error reading value from input file");
  }
}

// Close the input file
fclose(input_file);

}
// Send the array size to the other process
MPI_Bcast(N, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

void partition_in_half(double* A, double** local_A, int N, int rank) {
// Send the second half to the other process
if (rank == 0) {
MPI_Send(A+N, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
} else {
MPI_Recv(*local_A, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}
}

double get_min(double* local_A, int N, int k) {
double min_val = 0;
#pragma omp parallel for reduction(min:min_val) num_threads(k)
for (int i = 0; i < N; i++) {
double a_i = local_A[i];
double b_i = 0;
for (int k = 0; k <= MAX; k++) {
double temp = sin(k * exp(sin(a_i * k)));
if (temp < b_i) {
b_i = temp;
}
}
if (b_i < min_val) {
min_val = b_i;
}
}
return min_val;
}

double final_min(double min_val, int rank) {
double global_min = 0;
if (rank == 0) {
double recv_val;
MPI_Recv(&recv_val, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
global_min = fmin(min_val, recv_val);
} else {
MPI_Send(&min_val, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
}
return global_min;
}

double find_final_min(double min_val, int rank) {
double global_min;
if (rank == 0) {
double recv_val;
MPI_Recv(&recv_val, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
global_min = fmin(min_val, recv_val);
} else {
MPI_Send(&min_val, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
}
return global_min;
}