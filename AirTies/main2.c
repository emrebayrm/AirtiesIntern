#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>
#include "matrix_lib.h"

long gtd_u()
{
    struct timeval time;
    if(gettimeofday( &time, 0 )) return -1;

    long cur_time = 1000000 * time.tv_sec + time.tv_usec;

    return cur_time;
}

int main(int argc ,char *argv[]) {
    int i;
    int dim = 100;
    srand(time(NULL));
    if(argc > 1){
        dim = atoi(argv[1]);
    }


    double **first, **second, **multiply;
    first = calloc(dim ,sizeof(double*));

    for(i = 0 ; i < dim ; i++)
    {
        first[i] = calloc(dim, sizeof(double));
    }

    generate_matrix(first,dim);
    //print_matrix(first,dim);
    second = calloc(dim ,sizeof(double*));

    for(i = 0 ; i < dim ; i++)
    {
        second[i] = calloc(dim, sizeof(double));
    }
    generate_matrix(first,dim);

    multiply = calloc(dim ,sizeof(double*));

    for(i = 0 ; i < dim ; i++)
    {
        multiply[i] = calloc(dim, sizeof(double));
    }
    long start,end;

    generate_matrix(second,dim);
    start = gtd_u();
    matmul(first,second,multiply,dim);
    end = gtd_u();

    print_matrix(multiply,dim);

    printf("\nrunning time = %lf \n",(end - start) / 1000000.0);
    return 0;
}