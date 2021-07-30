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

#define LIMIT 200

int customCnt(char *fl);
void neededCnt(int x, int y, int **alloc, int max[x][y], int **needed);
void currReCnt();
int banker(int q, int w, int **alloc, int max[q][w], int avail[q], int **needed, int temparr[w]);
void *runTh(void *l);

int main(int argc, char *argv[]){
    if (argc < 2) { 
	    printf("Error - missing data for 'available' resources\n");
	    return -1; 
    }
	
    int avlbl_sze = argc - 1;


    
    int csts = customCnt("sample4_in.txt") + 1;
    printf("Number of Customers: %d\n", csts);


   
    custom = (Cst*)malloc(sizeof(Cst));
    custom->data_length = avlbl_sze;
    custom->csts = csts;

    avail = (int*)malloc(avlbl_sze * sizeof(int));
    if (avail == NULL) { 
        printf("Error - memory allocation failed\n"); 
        return -1;
    }

    
    for (int x = 0; x < avlbl_sze; x++) { 
        avail[x] = atoi(argv[x + 1]);
    }

   
    alloc = (int **)malloc(csts * sizeof(int *));

    for (int x = 0; x < csts; x++) { 
        alloc[x] = (int *)malloc(avlbl_sze * sizeof(int)); 
    }

    
    for (int p = 0; p < csts; p++) { 
        for (int q = 0; q < avlbl_sze; q++) { 
            alloc[p][q] = 0;
        }
    }

    
    needed = (int **)malloc(csts * sizeof(int *));

    for (int x = 0; x < csts; x++) { 
        needed[x] = (int *)malloc(avlbl_sze * sizeof(int));
    }
    int temparr[csts]; 


    
    int max[csts][avlbl_sze];

    for(int p = 0; p < csts; p++) { 
        for(int q = 0; q < avlbl_sze; q++) { 
            max[p][q] = 0;
        }
    }
    
    
	FILE *in = fopen("sample4_in.txt", "r");
	
	if(!in)
	{ 
		printf("Error opening file\n");
		return -1;
	}

	struct stat lp;
	fstat(fileno(in), &lp);
	char* fileContent = (char*)malloc(((int) lp.st_size + 1)* sizeof(char));
	fileContent[0]='\0';	
	
	
	while(!feof(in)) 
	{
		char line[100];
		if(fgets(line, 100, in) != NULL)
			strncat(fileContent, line, strlen(line));
	}

	fclose(in);

	char* lines[csts];
	char *cmmd = NULL;
	int x = 0;
	cmmd = strtok(fileContent,"\r\n");

    
    while(cmmd!=NULL)
	{
		lines[x] = malloc(sizeof(cmmd)*sizeof(char));
		strcpy(lines[x],cmmd);
		x++;
		cmmd = strtok(NULL,"\r\n");
	}

	
	for(int p = 0; p < csts; p++)
	{
		char* tkn = NULL;
		int q = 0;
		tkn =  strtok(lines[p],",");

		while(tkn!=NULL)
		{
			max[p][q] = atoi(tkn);
			q++;
			tkn = strtok(NULL,",");
		}
	}

   

    printf("Currently Available resources:");

	for (int p = 0; p < avlbl_sze; p++)
		printf(" %d", avail[p]);

	printf("\n");


    

    printf("Maximum resources from file:\n");

	for (int p = 0; p < csts; p++) {
    	for (int q = 0; q < avlbl_sze; q++) {
			printf("%d", max[p][q]);
			
			if (q != avlbl_sze - 1)
				printf(",");
    	}

		printf("\n");
	}

    
    char strngtn[LIMIT];
    char cmp[LIMIT];
    char rqrd[LIMIT] = "RQ";
    char rels[LIMIT] = "RL";
    char strng[LIMIT] = "Status\n";
    char runtn[LIMIT] = "Run\n";
    char extprog[LIMIT] = "exit\n";

    
    printf("Enter Command: ");
	fgets(strngtn, LIMIT, stdin);
	strcpy(cmp, strngtn);
	int kaambhari = 0;

    
    while (strcmp(cmp, extprog) != 0) {
        if (kaambhari > 0) {
            printf("Enter Command: ");
            fgets(strngtn, sizeof strngtn, stdin);
            strcpy(cmp, strngtn);
        }

		
		int fnl_strng = 0;
		for (int v = 0; cmp[v] != '\0'; v++)
		{
			if (cmp[v] == ' ' || cmp[v] == '\n' || cmp[v] == '\t')
				fnl_strng++;
		}

        
        char *tknt = strtok(cmp, " ");
		char *inpt_strng[LIMIT];
		
		int p =0;
		if (fnl_strng >= 2)
		{
            while (tknt != NULL && p <= csts) {
                inpt_strng[p] = tknt;
				tknt = strtok(NULL, " ");
				p+= 1;
			}
		}

        else
			strcpy(inpt_strng[0], cmp);
		
		int strng_lngth = p;
		p = 0;

        

        if (strcmp(inpt_strng[0], rqrd) == 0) {
			if (atoi (inpt_strng[1]) >= csts) { 
				printf("Index cannot be greater than customers\n");
			}

			else {
				for (int n = 2; n < (strng_lngth); n++) 
					alloc[atoi(inpt_strng[1])][n-2] = atoi(inpt_strng[n]); 

				printf("State is safe, and request is satisfied\n");
			}
		}


        
		else if (strcmp(inpt_strng[0], rels) == 0)
        {
            int fnsh_flg; 

			if (atoi (inpt_strng[1]) >= csts) {
				printf("Index cannot be greater than customers\n");
				
			}

			else {

				for (int d = 2; d < (strng_lngth); d++)
				{
					int rls_val;
					rls_val = alloc[atoi(inpt_strng[1])][d - 2] - atoi(inpt_strng[d]); 

					if (rls_val < 0) { 
						printf("unknown release\n");
						fnsh_flg = 1; 
						break;
					}
					
					else
						alloc[atoi(inpt_strng[1])][d - 2] = rls_val; 

					if ((d = strng_lngth - 1))
						printf("Release is satisfied\n");
				}

				

				if (fnsh_flg == 1)
					continue;
			}
        }

        
	
		else if (strcmp(inpt_strng[0], strng) == 0) {


			
			printf("Available Resources: ");

			currReCnt();


			for (int p = 0; p < avlbl_sze; p++)
				printf("%d ", avail[p]);


			printf("\n\n");
			

			
			printf("Maximum Resources: \n");
			
			for (int q = 0; q < csts; q++) {
				for (int v = 0; v < avlbl_sze; v++)
					printf("%d ", max[q][v]);

				printf("\n");
			}


			printf("\n");


			
			printf("Allocated Resources: \n");

			for (int p = 0; p < csts; p++) {
				for (int q = 0; q < avlbl_sze; q++)
					printf("%d ", alloc[p][q]);
				
				printf("\n");
        	}


			printf("\n");
		

			
			printf("Need Resources: \n");
			
			neededCnt(avlbl_sze, csts, alloc, max, needed);

			for (int p = 0; p < csts; p++) {
				for (int q = 0; q < avlbl_sze; q++)
					printf("%d ", needed[p][q]);

				printf("\n");
			}

			printf("\n");
		}

        
        		
		else if (strcmp(inpt_strng[0], runtn) == 0) {
			
			neededCnt(avlbl_sze, csts, alloc, max, needed);
			int rspnd = banker(avlbl_sze, csts, alloc, max, avail, needed, temparr);
			
			if (rspnd != 0) {
				printf("Could not find Safe sequence\n");
				return -1;
			}

			for (int t = 0; t < csts; t++) {
				int idxfd = temparr[t];
				printf("--> Customer/Thread %d\n", idxfd);
				
				
				pthread_t tid;
				pthread_create(&tid, NULL, runTh, &idxfd);
				pthread_join(tid, NULL);
			}
		}
		
		kaambhari += 1;
	}



	
	free(avail);
	free(alloc);
	free(needed);

	return 0;


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



