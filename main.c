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
//initilization 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>

//init of the interface
typedef struct cst {
    int csts;
    int data_length;
} Cst;

//initilization of the variables in accordance to pointer
int *avail;
int **alloc;
int **needed;

Cst *custom;

#define LIMIT 200

//definitions 
int customCnt(char *fl);
void neededCnt(int x, int y, int **alloc, int max[x][y], int **needed);
void currReCnt();
int banker(int q, int w, int **alloc, int max[q][w], int avail[q], int **needed, int temparr[w]);
void *runTh(void *l);


//main function
int main(int argc, char *argv[]){
    if (argc < 2) { //if loop
	    printf("Error - missing data for 'available' resources\n");//print
	    return -1; //return statement
    }
	
    int avlbl_sze = argc - 1; //init variable. this will calculate the available size


    
    int csts = customCnt("sample4_in.txt") + 1; //initilization of variable to find the given csts (customers) in a file and to return it in such a manner. 
    printf("Number of Customers: %d\n", csts);//print statement


   //where we are initliazating the arrays in accordance to the pointer values and location of the variable. 
    custom = (Cst*)malloc(sizeof(Cst));
    custom->data_length = avlbl_sze;
    custom->csts = csts;

    avail = (int*)malloc(avlbl_sze * sizeof(int));
    if (avail == NULL) { 
        printf("Error - memory allocation failed\n"); 
        return -1;
    }

    //argc values play a role in initializing the array values
    for (int x = 0; x < avlbl_sze; x++) { 
        avail[x] = atoi(argv[x + 1]);
    }

   //allocation of the two dimensional arrays 
    alloc = (int **)malloc(csts * sizeof(int *));

    for (int x = 0; x < csts; x++) { 
        alloc[x] = (int *)malloc(avlbl_sze * sizeof(int)); 
    }

    //initliazing the allocation of arrays to a null value of zero
    for (int p = 0; p < csts; p++) { 
        for (int q = 0; q < avlbl_sze; q++) { 
            alloc[p][q] = 0;
        }
    }

    //this portion we will do allocation of the two dimensional array for the portion of code that is needed
    needed = (int **)malloc(csts * sizeof(int *));

//for loop to init
    for (int x = 0; x < csts; x++) { 
        needed[x] = (int *)malloc(avlbl_sze * sizeof(int));
    }
    int temparr[csts]; //we will create the temporary array


    
    int max[csts][avlbl_sze];//initialize a maximum array that will be used to read the data from the file and will insert into such
//for loop conditional that will init the max arrays into null values. 
    for(int p = 0; p < csts; p++) { 
        for(int q = 0; q < avlbl_sze; q++) { 
            max[p][q] = 0;
        }
    }
    
    //open the file 
    FILE *in = fopen("sample4_in.txt", "r");
//this will check if the file is opened or not. 
    if(!in) { 
	    printf("Error opening file\n");
	    return -1;
    }

    struct stat lp;
    fstat(fileno(in), &lp);
    char* fileContent = (char*)malloc(((int) lp.st_size + 1)* sizeof(char));
    fileContent[0]='\0';	
	
//storing the data from the file into the filecontent string 
    while(!feof(in)) {
	    char line[100];
	    if(fgets(line, 100, in) != NULL)
		    strncat(fileContent, line, strlen(line));
    }

    fclose(in);//close such file
//init the array vars
    char* lines[csts];
    char *cmmd = NULL;
    int x = 0;
    cmmd = strtok(fileContent,"\r\n");

//whie loop that will basically take the data from the filecontent and store to the lines array    
    while(cmmd!=NULL) {
	lines[x] = malloc(sizeof(cmmd)*sizeof(char));
	strcpy(lines[x],cmmd);
	x++;
	cmmd = strtok(NULL,"\r\n");
    }

//for loop that is used to basically take the token of each line and to add the singular numbers to array	
    for(int p = 0; p < csts; p++){
	char* tkn = NULL;
	int q = 0;
	tkn =  strtok(lines[p],",");
//while loop
	while(tkn!=NULL) {
		max[p][q] = atoi(tkn);
		q++;
		tkn = strtok(NULL,",");
	}
    }

   
//print statement for the current available resources 
    printf("Currently Available resources:");
//for loop for conditional 
    for (int p = 0; p < avlbl_sze; p++)
	printf(" %d", avail[p]);

    printf("\n");


    
//print statement for the maximum resources from the file 
    printf("Maximum resources from file:\n");
//for loop 
   for (int p = 0; p < csts; p++) {
    	for (int q = 0; q < avlbl_sze; q++) {
		printf("%d", max[p][q]);
			
		if (q != avlbl_sze - 1)
			printf(",");
    	}
//print line 
	printf("\n");
    }

//init variables for the character arrays (command string)
    char strngtn[LIMIT];
    char cmp[LIMIT];
    char rqrd[LIMIT] = "RQ";
    char rels[LIMIT] = "RL";
    char strng[LIMIT] = "Status\n";
    char runtn[LIMIT] = "Run\n";
    char extprog[LIMIT] = "exit\n";

//print statement for asking for initial request to initially run while loop 
    printf("Enter Command: ");
    fgets(strngtn, LIMIT, stdin);
    strcpy(cmp, strngtn);
    int kaambhari = 0;

//programs the main logic when the exit has not been initiated         
    while (strcmp(cmp, extprog) != 0) {
        if (kaambhari > 0) {
            printf("Enter Command: ");
            fgets(strngtn, sizeof strngtn, stdin);
            strcpy(cmp, strngtn);
        }

	//we are bound to be string spilliting in such conditionals 	
	int fnl_strng = 0;
	for (int v = 0; cmp[v] != '\0'; v++) {
		if (cmp[v] == ' ' || cmp[v] == '\n' || cmp[v] == '\t')
			fnl_strng++;
	}

        //initalize the parsing of the logic
        char *tknt = strtok(cmp, " ");
	char *inpt_strng[LIMIT];
	//init	
	int p =0;
	//if conditional    
	if (fnl_strng >= 2) {
            while (tknt != NULL && p <= csts) {
                inpt_strng[p] = tknt;
		tknt = strtok(NULL, " ");
		p+= 1;
	    }
	}
	//else conditional 
        else
		strcpy(inpt_strng[0], cmp);
	//init	
	int strng_lngth = p;
	p = 0;
	//end of parsing in such 
        
	//we will have COMMAND: resource request in such
        if (strcmp(inpt_strng[0], rqrd) == 0) {
		if (atoi (inpt_strng[1]) >= csts) { //idiot proofing of the resource request command
			printf("Index cannot be greater than customers\n"); //print statement + continuation 
		}
		//else conditional
		else { 
			for (int n = 2; n < (strng_lngth); n++) //for conditional to store req resource data
				alloc[atoi(inpt_strng[1])][n-2] = atoi(inpt_strng[n]); //convert alloc resource info from str to int

				printf("State is safe, and request is satisfied\n"); //print statement
		}
	}


        //else conditional for command resource release 
	else if (strcmp(inpt_strng[0], rels) == 0) {
        	int fnsh_flg; //init the request release not satisfied val

		if (atoi (inpt_strng[1]) >= csts) { //idiot proof for resource req
			printf("Index cannot be greater than customers\n");//print statement + continuation
				
		}
		//else conditional
		else {
			//for conditional
			for (int d = 2; d < (strng_lngth); d++) {
				int rls_val; //int 
				rls_val = alloc[atoi(inpt_strng[1])][d - 2] - atoi(inpt_strng[d]); //calc the release val from alloc  two dimensional array

				if (rls_val < 0) { //idiot proof the resource releast, to avoid alloc res val below null zero
					printf("unknown release\n");//print
					fnsh_flg = 1; //init
					break;//break
				}
					
				else //else conditional
					alloc[atoi(inpt_strng[1])][d - 2] = rls_val; //changing the location of val for the pointed rel val at current time

				if ((d = strng_lngth - 1))
					printf("Release is satisfied\n");
				}

				
			//we will leave the loop to start over from while as we reject value of what we have put in
			if (fnsh_flg == 1) //if conditional 
				continue; //continue
		}
        }

        
	//else if conditional for command at display detail
	else if (strcmp(inpt_strng[0], strng) == 0) {
		printf("Available Resources: "); //print the currently avail resource

		currReCnt();

		//for conditional
		for (int p = 0; p < avlbl_sze; p++)
			printf("%d ", avail[p]);

		//print line
		printf("\n\n");
			

		//print the maximum amount of resources
		printf("Maximum Resources: \n");
		//for loop
		for (int q = 0; q < csts; q++) {
			for (int v = 0; v < avlbl_sze; v++)
				printf("%d ", max[q][v]); //print statement

			printf("\n"); //print statement
		}


		printf("\n"); //print line 


			
		printf("Allocated Resources: \n"); //print the allocated resources in such
		//for conditional
		for (int p = 0; p < csts; p++) {
			for (int q = 0; q < avlbl_sze; q++)
				printf("%d ", alloc[p][q]); //print val 
				
			printf("\n"); //print line
        	}


		printf("\n"); //print line
		

			
		printf("Need Resources: \n"); //print the need resources
			
		neededCnt(avlbl_sze, csts, alloc, max, needed);

		for (int p = 0; p < csts; p++) {
			for (int q = 0; q < avlbl_sze; q++)
				printf("%d ", needed[p][q]); //print the val

			printf("\n"); //print line
		}

			printf("\n"); //print line
	}

        
        //else statement + command run of the threads to find a safe seq
	else if (strcmp(inpt_strng[0], runtn) == 0) {
		printf("Executing run command\n"); //print command
		
		neededCnt(avlbl_sze, csts, alloc, max, needed);
		int rspnd = banker(avlbl_sze, csts, alloc, max, avail, needed, temparr); //init
			
		if (rspnd != 0) {
			printf("Could not find Safe sequence\n");
			return -1;
		}

		for (int t = 0; t < csts; t++) {
			int idxfd = temparr[t];
			printf("--> Customer/Thread %d\n", idxfd); //print customer thread val
				
			//running the thread
			pthread_t tid;
			pthread_create(&tid, NULL, runTh, &idxfd);
			pthread_join(tid, NULL);
		}
	}
		
	kaambhari += 1; //+=1 to work field
    }



	//we have put in the free dynamic stored val of the data
	free(avail);
	free(alloc);
	free(needed);

	return 0; //return val


}
// This function is created for the Bankers Algorithm
int banker(int q, int w, int **alloc, int max[w][q], int avail[q], int **needed, int temparr[w]){
    // Declare init variables
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
                // declare indicator as flag
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
    // Obtain safe sequence
    for (d = 0; d < w; d++)
    {
        if (n[d] == 0)
            sSeq = 0;
    }
    // If safe sequence is not valid, return -1
    if (sSeq == 0)
        return -1;
    // Print the safe sequence
    printf("Safe Sequence is: <");

    for (d = 0; d < w - 1; d++)
        printf("%d ", temparr[d]);

    printf("%d>\n", temparr[w - 1]);

    return 0;
}
// This function calculates the current existing resources
void currReCnt() {
    // Declare temporary variables
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

// This function would obtain the times new line has occured in the file
int customCnt(char *fl) {
    // declaring vars, integer cnt, and character n
    FILE *pointer;
    int cnt = 0;
    char n;
    // open fl
    pointer = fopen(fl, "r");
    // Obtain the characters from the file pointer and save in 'n'
    n = getc(pointer);
    
	while (n != EOF)
    {
	// increment count when new line is there
        if (n == '\n')
            cnt++;

        n = getc(pointer);
    }
    // close file
    fclose(pointer);
    return cnt; // return the number of new lines in the file
}
// This function obtains the number of resources needed.
void neededCnt(int x, int y, int **alloc, int max[y][x], int **needed) {
	// the nested for loop creates the needed resources matrix.
	for (int k = 0; k < y; k++) { 
        	for (int l = 0; l < x; l++) { 
            		needed[k][l] = max[k][l] - alloc[k][l];
        	}
	}
}

// This function is made for running thread
void *runTh(void *l){

    int *clnttkr = (int *)l; 
    // Prints allocated reosources
    printf("\tAllocated Resources:\t");
	for (int c = 0; c < custom->data_length; c++)
		printf("%d ", alloc[*clnttkr][c]);
	

	printf("\n");

    // Prints needed resources
    printf("\tNeeded:\t");
    for (int v = 0; v < custom->data_length; v++){
        printf("%d ", needed[*clnttkr][v]);
    }
    printf("\n");

    // prints availale resources
    printf("\tAvailable:\t");
    for (int v = 0; v < custom->data_length; v++){
        printf("%d ", avail[v]);
    }

    // New line
    printf("\n");


    printf("\tThread has started\n");
    printf("\tThread has finished\n");
    printf("\tThread is realeasing resources\n");
    
    // Prints newly available resources
    printf("\tNew Available:\t");
    
    for (int v =0; v < custom->data_length; v++){
    	avail[v] =  avail[v] + alloc[*clnttkr][v];
        printf("%d ", avail[v]);
    }

    printf("\n");
    return NULL;

}



