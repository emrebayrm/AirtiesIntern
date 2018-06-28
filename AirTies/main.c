//
// Created by patates on 6/27/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

void print_maze(char **m,int size);
int solve_dead_end_fill(char ** m, int size);
void fix_maze(char **m,int size);
int main(int argc,char* argv[]){
    FILE *inp;

    char **maze;
    char buffer[1024];
    char *buf;
    int size;
    int bufsize = 1024;
    int i = 1;
    inp = fopen(argv[1],"r");
    buf = buffer;
    if(inp == NULL)
        return 1;
    size = getline(&buf,&bufsize,inp);

    maze = calloc(size,sizeof(char*));
    maze[0] = calloc(size,sizeof(char));
    memcpy(maze[0],buffer,size);
    maze[0][63]='\0';

    while(!feof(inp)){
        maze[i] = calloc(size+1, sizeof(char));
        getline(&maze[i],&size,inp);
        maze[i][63]='\0';
        i++;
    }

    fix_maze(maze,size);
    print_maze(maze,size);
    solve_dead_end_fill(maze,size);
    printf("\n\n\n");
    print_maze(maze,size);
    return 0;
}
int solve_dead_end_fill(char ** m, int size){
    int isChanged = 1;
    int count = 0;
    while(isChanged) {
        isChanged = 0;
        for (int i = 1; i < size -1; ++i) {
            for (int j = 1; j < size-1; ++j) {
                if(m[i][j] == 0) {
                    if (m[i + 1][j] + m[i - 1][j] + m[i][j + 1] == 3) {
                        isChanged++;
                        m[i][j] = 1;
                    }
                    if (m[i + 1][j] + m[i - 1][j] + m[i][j - 1] == 3) {
                        isChanged++;
                        m[i][j] = 1;
                    }
                    if (m[i][j + 1] + m[i][j - 1] + m[i + 1][j] == 3) {
                        isChanged++;
                        m[i][j] = 1;
                    }
                    if (m[i][j + 1] + m[i][j - 1] + m[i - 1][j] == 3) {
                        isChanged++;
                        m[i][j] = 1;
                    }
                }
            }
        }
        ++count;
    }
    printf("%d",count);
    return 1;
}

void print_maze(char **m,int size){
    for (int i = 0; i < size ; ++i) {
        for (int j = 0; j < size; ++j) {
            if( m[i][j] == 1)
                printf("X");
            else
                printf(" ");
        }
        printf(" %d \n",i);
    }
}

void fix_maze(char **m,int size){
    for (int i = 0; i < size ; ++i) {
        for (int j = 0; j < size; ++j) {
            if(m[i][j] == 'X')
                m[i][j] = 1;
            else
                m[i][j] = 0;
        }
    }
}