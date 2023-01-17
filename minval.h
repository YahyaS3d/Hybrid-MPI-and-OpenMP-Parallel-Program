#ifndef MINVAL_H
#define MINVAL_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>

#define MAX 10000
#define FILE_NAME "input.txt"
#define OMP_NUM_THREADS 2

void partition_in_half(double*,double**,int,int);
double find_final_min(double,int);
double get_min(double*,int,int);
void first_proccess_read(int*,double**,int);
void error(char*);
#endif




