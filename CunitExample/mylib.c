//
// Created by patates on 7/4/18.
//

#include "mylib.h"

int find(int *arr, int size, int b){
    for (int i = 0; i < size; ++i) {
        if(arr[i] == b )
            return i;
    }
    return -1;
}