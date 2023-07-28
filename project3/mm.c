#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "mm.h"



// Task 1: Flush the cache so that we can do our measurement :)
void flush_all_caches()
{
	//printf("huge_matrixA is at address %p \n",huge_matrixA);
	//printf("end of huge_matrixA is %p \n", huge_matrixA + (sizeof(long)*(long)SIZEX*(long)SIZEY-1) + 1); 
	__builtin___clear_cache(huge_matrixA, huge_matrixA + (sizeof(long)*(long)SIZEX*(long)SIZEY-1) + 1); 
	__builtin___clear_cache(huge_matrixB, huge_matrixB + (sizeof(long)*(long)SIZEX*(long)SIZEY-1) + 1);
	__builtin___clear_cache(huge_matrixC, huge_matrixC + (sizeof(long)*(long)SIZEX*(long)SIZEY-1) + 1);

}

void load_matrix_base()
{
	long i;
	huge_matrixA = malloc(sizeof(long)*(long)SIZEX*(long)SIZEY);
	huge_matrixB = malloc(sizeof(long)*(long)SIZEX*(long)SIZEY);
	huge_matrixC = malloc(sizeof(long)*(long)SIZEX*(long)SIZEY);
	// Load the input
	// Note: This is suboptimal because each of these loads can be done in parallel.
	for(i=0;i<((long)SIZEX*(long)SIZEY);i++)
	{
		fscanf(fin1,"%ld", (huge_matrixA+i)); 		
		fscanf(fin2,"%ld", (huge_matrixB+i)); 		
		huge_matrixC[i] = 0;		
	}
}

void free_all()
{
	free(huge_matrixA);
	free(huge_matrixB);
	free(huge_matrixC);
}

void multiply_base()
{
	
	long a, b, c; 

    for (a = 0; a < (long)SIZEY; a++) {
        for (b = 0; b < (long)SIZEY; b++) {
            for (c = 0; c < (long)SIZEY; c++) {
				long indx1 = a*(long)SIZEY+b; 
                long indx2 = a*(long)SIZEY+c; 
                long indx3 = c*(long)SIZEY+b; 
                huge_matrixC[indx1] +=  huge_matrixA[indx2] * huge_matrixB[indx3]; 
            }
        }
    } 

	// printf("Output \n"); 

    // for (int i = 0; i < ((long)SIZEX*(long)SIZEY); i++) { 
    //     printf("%d \n", huge_matrixC[i]); 
    // } 
	
}

void compare_results()
{
	fout = fopen("./out.in","r");
	long i;
	long temp1, temp2;
	for(i=0;i<((long)SIZEX*(long)SIZEY);i++)
	{
		fscanf(fout, "%ld", &temp1);
		fscanf(ftest, "%ld", &temp2);
		if(temp1!=temp2)
		{
			printf("Wrong solution!");
			exit(1);
		}
	}
	fclose(fout);
	fclose(ftest);
}

void write_results()
{
	if (fout == NULL)
    {
        printf("Error opening the file %s", "./input2.in");
        return -1;
    }

	for (int i = 0; i < ((long)SIZEX*(long)SIZEY); i++) { 
        fprintf(fout, "%d \n", huge_matrixC[i]); 
    } 
}

void load_matrix()
{
	fin1 = fopen("./input1.in","r");
	fin2 = fopen("./input2.in","r");
	fout = fopen("./out.in","w");
	ftest = fopen("./reference.in","r");
	load_matrix_base(); 
}



void multiply(){

	long i, j, k, kk, jj;
	long sum; 
	int en = (SIZEY/2) * (SIZEY/(SIZEY/2)); 

	// printf("hell0 \n"); 

	for (kk = 0; kk < en; kk += SIZEY/2) {
		for (jj = 0; jj < en; jj += SIZEY/2) {
			for (i = 0; i < SIZEY; i++) {
				for (j = jj; j < jj + (SIZEY/2); j++) {
					sum = huge_matrixC[i*(long)SIZEY+j];
					for (k = kk; k < kk + (SIZEY/2); k++) {
						sum += huge_matrixA[i*(long)SIZEY+k] * huge_matrixB[k*(long)SIZEY+j]; 
					}
					huge_matrixC[i*(long)SIZEY+j] = sum;
				}
			}
		}		
	}

	// for (int i = 0; i < ((long)SIZEX*(long)SIZEY); i++) { 
    //     printf("%d \n", huge_matrixA[i]); 
    // } 

	// printf("matrix B \n"); 

	// for (int i = 0; i < ((long)SIZEX*(long)SIZEY); i++) { 
    //     printf("%d \n", huge_matrixB[i]); 
    // } 

	// for (int i = 0; i < ((long)SIZEX*(long)SIZEY); i++) { 
    //     printf("%d \n", huge_matrixC[i]); 
    // } 
}

int main()
{
	
	clock_t s,t;
	double total_in_base = 0.0;
	double total_in_your = 0.0;
	double total_mul_base = 0.0;
	double total_mul_your = 0.0;
	fin1 = fopen("./input1.in","r");
	fin2 = fopen("./input2.in","r");
	fout = fopen("./out.in","w");
	ftest = fopen("./reference.in","r");
	
	flush_all_caches();

	s = clock();
	load_matrix_base();
	t = clock();
	total_in_base += ((double)t-(double)s) / CLOCKS_PER_SEC;
	printf("[Baseline] Total time taken during the load = %f seconds\n", total_in_base);

	s = clock();
	multiply_base();
	t = clock();
	total_mul_base += ((double)t-(double)s) / CLOCKS_PER_SEC;
	printf("[Baseline] Total time taken during the multiply = %f seconds\n", total_mul_base);
	fclose(fin1);
	fclose(fin2);
	fclose(fout);
	free_all();

	flush_all_caches();

	s = clock();
	load_matrix();
	t = clock();
	total_in_your += ((double)t-(double)s) / CLOCKS_PER_SEC;
	printf("Total time taken during the load = %f seconds\n", total_in_your);

	s = clock();
	multiply();
	t = clock();
	total_mul_your += ((double)t-(double)s) / CLOCKS_PER_SEC;
	printf("Total time taken during the multiply = %f seconds\n", total_mul_your);
	write_results();
	fclose(fin1);
	fclose(fin2);
	fclose(fout);
	free_all();
	compare_results();

	return 0;
}
