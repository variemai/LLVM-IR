#include <mpi.h>
#include <cstdio>
#define ARRAY_SIZE 1000

int main(int argc, char *argv[]){
    int rank;
    double array[ARRAY_SIZE];
    int row = 0;
    volatile double sum = 0.0;

    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    array[row] = 1.0;
    for (row=1; row<ARRAY_SIZE; row++)
        array[row] = array[row-1] + 1.0;


    for (row=1; row<ARRAY_SIZE; row++)
        sum = array[row]*array[row-1];

    printf("Rank = %d, Sum = %.3lf\n",rank,sum);
    MPI_Finalize();
    return 0;
}
