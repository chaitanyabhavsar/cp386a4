/*
 -------------------------------------
 File:    main.c
 Project: CP386A4
 
 -------------------------------------
 Author:  Jiten Aylani, Chaitanya Bhavsar
 ID:      190469460 , 190427300 
 Email:   ayla9460@mylaurier.ca , bhav7300@mylaurier.ca
 Version  2021/07/26
 -------------------------------------
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
typedef struct cst {
    int csts;
    int data_length;
} Cst;
int *avail;
int **alloc;
int **needed;

Cst *custom;

int customCnt(char *fl);
void neededCnt(int x, int y, int **alloc, int max[x][y], int **needed);
void currReCnt();
int banker(int q, int w, int **alloc, int max[q][w], int avail[q], int **needed, int temparr[w]);
void *runTh(void *l);

int main(int argc, char *argv[]){
}
int banker(int q, int w, int **alloc, int max[w][q], int avail[q], int **needed, int temparr[w]){
    int d, s, u;
    int sSeq = 1;

    int n[w], dex = 0;
    for (u = 0; u < w; u++)
        n[u] = 0;

    int z = 0;
    for (u = 0; u < q; u++)
    {
        for (d = 0; d < w; d++)
        {
            if (n[d] == 0)
            {

                int indicator = 0;
                for (s = 0; s < q; s++)
                {
                    if (needed[d][s] > avail[s])
                    {
                        indicator = 1;
                        break;
                    }
                }

                if (indicator == 0)
                {
                    temparr[dex++] = d;

                    for (z = 0; z < q; z++)
                        avail[z] += alloc[d][z];

                    n[d] = 1;
                }
            }
        }
    }

    for (d = 0; d < w; d++)
    {
        if (n[d] == 0)
            sSeq = 0;
    }

    if (sSeq == 0)
        return -1;

    printf("Safe Sequence is: <");

    for (d = 0; d < w - 1; d++)
        printf("%d ", temparr[d]);

    printf("%d>\n", temparr[w - 1]);
	printf("Now going to executing the threads:\n\n\n");

    return 0;
}
void currReCnt() {

    int tmprpy; 
    int tmprpy2;

    for (int k = 0; k < custom->data_length; k++) { 
        tmprpy = 0;

        for (int m = 0; m < custom->csts; m++) { 
            tmprpy = tmprpy + alloc[m][k];
        }

        tmprpy2 = avail[k] - tmprpy;
        avail[k] = tmprpy2;
    }
}

int customCnt(char *fl) {
	
	FILE *pointer;
    int cnt = 0;
    char n;
 
    pointer = fopen(fl, "r");

    n = getc(pointer);
    
	while (n != EOF)
    {
		
        if (n == '\n')
            cnt++;

        n = getc(pointer);
    }

    fclose(pointer);
    return cnt;
}

void neededCnt(int x, int y, int **alloc, int max[y][x], int **needed) {
	for (int k = 0; k < y; k++) { 
        	for (int l = 0; l < x; l++) { 
            		needed[k][l] = max[k][l] - alloc[k][l];
        	}
	}
}

void *runTh(void *l){

    int *clnttkr = (int *)l; 
    
    printf("\tAllocated Resources:\t");
	for (int c = 0; c < custom->data_length; c++)
		printf("%d ", alloc[*clnttkr][c]);
	

	printf("\n");


   	printf("\tNeeded:\t");
    for (int v = 0; v < custom->data_length; v++){
        printf("%d ", needed[*clnttkr][v]);
    }
	printf("\n");


    printf("\tAvailable:\t");
    for (int v = 0; v < custom->data_length; v++){
        printf("%d ", avail[v]);
    }


    printf("\n");


    printf("\tThread has started\n");
    printf("\tThread has finished\n");
    printf("\tThread is realeasing resources\n");
    

	printf("\tNew Available:\t");
    
	for (int v =0; v < custom->data_length; v++){
        avail[v] =  avail[v] + alloc[*clnttkr][v];
        printf("%d ", avail[v]);
    }

    printf("\n");
	
	return NULL;

}



