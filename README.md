# K_Means_Parallel_Implimentation

## To build the sequential part use the command-
```
gcc -fopenmp lab1_io.c main_sequential.c lab1_sequential.cpp -o seq
```

## To build the pthreads part use the command-
```
gcc -fopenmp lab1_io.c main_pthread.c lab1_pthread.cpp -o pthread
```

## To build the OpenMP part use the command-
```
gcc -fopenmp lab1_io.c main_omp.c lab1_omp.cpp -o omp
```
