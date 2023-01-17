Calculate a minimal value of the very large array of values using MPI + OpenMP environment. 
Name: Yahya Saad
==Description==
This will run the program with 2 processes. The value of MAX can be changed in the minval.h file. The input values are read from the file "input.txt".
The program return the minimum value of a large array after manipulation a complex math equation using: MPI and OpenMP with 2 processes and X threads.

|    omp threads    |    exec time    |   total threads|              explanation           |
| :---------------: |:---------------:|:--------------:| :---------------------------------:|
|         2         |     0.972332    |        4       |  Too few threads - 12 idle         |
|         4         |     0.523334    |        8       |  Too few threads - 8 idle          |
|         16        |     0.401291    |        16      |  Best fit - 0 idle                 |
|         16        |     0.421738    |        32      |  Too many threads - thread racing  |

==Files== 
* minval.c - code for parallel summerize with mpi and omp 
* function.c - functions declaration  
* minval.h - contains the header of minval.c, functions and macros 
* input.txt - text file containing the floats for process
* README.txt- readme for this program
 ==COMPILATION==
> to run the above files mpich must be installed on your machine,
    in project directory run:
> * ```mpicc -fopenmp minval.c function.c -o exec_name -lm```

==EXECUTION==
> to execute compiled files you need to run 2 proccess and K max iteration for calculation:
> * ```mpiexec -np 2 -env OMP_NUM_THREADS 8 ./exec_name```
==Output==
Time: exec time for the program 
minimum value of the array which calculated 