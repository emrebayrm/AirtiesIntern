#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum{TRUE, FALSE} BOOLEAN;

void printFile(char *name,int screenSize,int start, BOOLEAN isShow,BOOLEAN isFold);

int main(int argc ,char *argv[]) {
    int screenSize = 80;
    int startline = 0;
    BOOLEAN isShowNumbers = FALSE;
    BOOLEAN isFold = FALSE;
    for (int i = 1; i < argc - 1; i++) {
        if (strncmp("+num",argv[i],4) == 0){
            screenSize = atoi(argv[i+1] );
            ++i;
        }
        else if(strncmp("-num",argv[i],4) == 0) {
            startline = atoi(argv[i+1] );
            ++i;
        }
        else if(strncmp("-N",argv[i],4) == 0){
            isShowNumbers = TRUE;
        }
        else if(strncmp("-F",argv[i],4) == 0) {
            isFold = TRUE;
        }
        else{
            printf("Error at opt %s",argv[i]);
            return 1;
        }
    }
    printf("options : %d , %d , %c , %c \n",screenSize,startline,isShowNumbers == FALSE ? 'h' : 'e', isFold == FALSE ? 'h' : 'e' );
    printFile(argv[argc-1],screenSize,startline,isShowNumbers,isFold);

    return 0;
}

int mystrlen(char *str){
    int i;
    for (i = 0;str[i] != '\0'; ++i) ;

    return i;
}

void printFile(char *name,int screenSize,int start, BOOLEAN isShowNumbers,BOOLEAN isFold){
    FILE *inp;

    if(name == NULL)
        inp = stdin;
    else
        inp = fopen(name,"r");
    if(inp == NULL)
        return;
    int i = 0,size = 1024;
    int len;
    char *buffer;
    buffer = (char *) malloc(1024);

    for (int j = 0; j < start; ++j) {
        getline(&buffer,&size,inp);
    }

    while(!feof(inp)){
        memset(buffer,0,1024);
        getline(&buffer,&size,inp);
        if(isShowNumbers == TRUE)
            printf("%d - ",i);
        len = mystrlen(buffer);
        int j;
        for (j = 0; j < len; ++j) {
            printf("%c",buffer[j]);
            if((j+1) % screenSize == 0) {
                if (isFold == TRUE)
                    printf("\n");
                else
                    break;
            }
        }
        if(j < len)
            printf("\n");

        i++;
    }
}