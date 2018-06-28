//
// Created by patates on 6/27/18.
//

#include <stdlib.h>
#include <stdio.h>
#include "matrix_lib.h"


void print_matrix(double ** mat,int dim){
    int i,j;
    for (i = 0; i < dim; ++i) {
        for (j = 0;  j < dim; ++j) {
            printf(" %lf ",mat[i][j]);
        }
        printf("\n");
    }
}

void generate_matrix(double **mat,int dim){
    int i,j;
    for (i = 0; i < dim; ++i) {
        for (j = 0;  j < dim; ++j) {
            mat[i][j] = (rand() % 100) / 100.0;
        }
    }
}

void matmul(double **a,double **b, double **c,int dim){
    int i,j,k;
    for (i = 0; i < dim; ++i) {
        for (k = 0; k < dim; ++k) {
            for (j = 0; j < dim; ++j) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}