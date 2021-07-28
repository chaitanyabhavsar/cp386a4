/*
 -------------------------------------
 File:    main.c
 Project: CP386A4
 
 -------------------------------------
 Author:  Jiten Aylani, Chaitanya Bhavsar
 ID:      190469460 , 
 Email:   ayla9460@wlu.ca , 
 Version  2021/07/26
 -------------------------------------
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct cst {
    int csts;
    int data_length;
} Cst;
int *avail;
int *alloc;
int *needed;

Cst *custom;

int banker(int q, int w, int **alloc, int max[q][w], int avail[q], int **needed, int temparr[w]);
void *runTh(void *l);


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
void *runTh(void *l){

    int *clnttkr = (int *)l; 
    
    printf("\tAllocated Resources:\t");
	for (int c = 0; c < custom->data_length-1; c++)
		printf("%d ", alloc[*clnttkr][c]);
	

	printf("\n");


   	printf("\tNeeded:\t");
    for (int v = 0; v < custom->data_length -1; v++){
        printf("%d ", needed[*clnttkr][v]);
    }
	printf("\n");


    printf("\tAvailable:\t");
    for (int v = 0; v < custom->data_length -1 ; v++){
        printf("%d ", avail[v]);
    }


    printf("\n");


    printf("\tThread has started\n");
    printf("\tThread has finished\n");
    printf("\tThread is realeasing resources\n");
    

	printf("\tNew Available:\t");
    
	for (int v =0; v < custom->data_length - 1; v++){
        avail[v] =  avail[v+1] + alloc[*clnttkr][v];
        printf("%d ", avail[v]);
    }

    printf("\n");
	
	return NULL;

}