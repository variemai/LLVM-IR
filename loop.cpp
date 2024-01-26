#include <cstdio>
#define ARRAY_SIZE 1000000

int main(int argc, char *argv[]){
    double array[ARRAY_SIZE];
    int row = 0;
    volatile double sum = 0.0;
    array[row] = 1.0;
    for (row=1; row<ARRAY_SIZE; row++)
        array[row] = array[row-1] + 1.0;


    for (row=0; row<ARRAY_SIZE; row++)
        sum += array[row];

    printf("Sum = %.3lf\n",sum);
    return 0;
}
