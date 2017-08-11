#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define EPS 2.0e-6
#define MAXM 100

#define NCOL 1
#define CHOSEN_COL 1

#define N1 1048576
#define PI 3.14159524288
#define RNG random()/2147483648.0
 
//---------------------------------------
int index_maximo(double *vector, int init, int end)
{
	int i, index;
	double max=vector[init];	
	for (i=init+1;i<=end;i++){
	if (vector[i]>=max){max=vector[i];index=i;}
	}
return(index);
}




double maximo(double *vector, int init, int end)
{
	int i;
	double max=vector[init];	
	for (i=init+1;i<=end;i++){
	if (vector[i]>=max){max=vector[i];}
	}
return(max);
}


normalize(double *vector, int size, double *out){
     	int i;
 	double max;
	max=maximo(vector,1,size);
	for(i=1;i<=size;i++) out[i]=vector[i]/max;
}



int maxindex(double *vector, int size)
{
	int i,index;
	double max;
	max=vector[1];
	for (i=2;i<=size;i++){
	if (vector[i]>max){max=vector[i];index=i;}
	}
return(index);
}


double minimo(double *vector, int size)
{
	int i;
	double min;	
	for (i=1;i<=size;i++){
	if (vector[i]<min){min=vector[i];}
	}
return(min);
}


int filesize(char name[], int ncol){
	int i, size = 0;
	double dato;
	FILE *datos;
	datos = fopen(name, "r");
	if(datos==NULL) {
       		printf("Missing file %s\n", name);
       		exit(0); 	
	}      
	while(!feof(datos)){
       		for(i=1; i<=ncol; i++) fscanf(datos, "%lg", &dato);       
       		size++;		
	}
	fclose(datos);
	return(size-1);
}



//---------------------------------------
void file_to_matrix(char *name, double **matrix, int start, int end, int ncols)
{
        int i, j;
        double datos1;
        FILE *datos;
        datos = fopen(name, "r");
        if(datos==NULL) {
            printf("Missing file %s... \nExiting to system\n\n", name);
            exit(0);
        }

        for(i=start; i<=end; i++){
                for (j=1; j<=ncols; j++) {
                        fscanf(datos, "%lg", &datos1);
                        matrix[i][j]= datos1;
                }
        }
        fclose(datos);
        return;
}
void file_to_matrix1(char *name, double **matrix, int start, int end, int ncols)
{
        int i, j;
        double datos1;
        FILE *datos;
        datos = fopen(name, "r");
        if(datos==NULL) {
            printf("Missing file %s... \nExiting to system\n\n", name);
            exit(0);
        }

        for(i=start; i<=end; i++){
                for (j=1; j<=ncols; j++) {
                        fscanf(datos, "%lg", &datos1);
                        matrix[i][j]= datos1;
                }
        }
        fclose(datos);
        return;
}

//---------------------------------------
void matrix_to_file(char *name, double **matrix, int start, int end, int Ncol)
{   
        int i,j;
        FILE *datos;
        datos = fopen(name, "w"); 
        for(i=start; i<=end; i++){ 
		for(j=0; j<Ncol; j++) fprintf(datos, "%lg\t", matrix[i][j]);
		fprintf(datos,"\n");
	}
        fclose(datos);
        return;
}   

void matrix_to_file1(char *name, double **matrix, int start, int end, int Ncol)
{   
        int i,j;
        FILE *datos;
        datos = fopen(name, "w"); 
        for(i=start; i<=end; i++){ 
		for(j=1; j<=Ncol; j++) fprintf(datos, "%lg\t", matrix[i][j]);
		fprintf(datos,"\n");
	}
        fclose(datos);
        return;
}   



//---------------------------------------
void file_to_vector(char *name, double vector[], int start, int end, int ncols,
    		    int chosen_col)
{
        int i, j;
        double datos1;
        FILE *datos;
        datos = fopen(name, "r");
        if(datos==NULL) {
            printf("Missing file %s... \nExiting to system\n\n", name);
            exit(0);
        }

        for(i=start; i<=end; i++){
                for (j=1; j<=ncols; j++) {
                        fscanf(datos, "%lg", &datos1);
                        if(j==chosen_col) vector[i]= datos1;
                }
        }
        fclose(datos);
        return;
}



//---------------------------------------
void file_to_ivector(char *name, int vector[], int size, int ncols,
                    int chosen_col)
{
        int i, j;
        double datos1;
        FILE *datos;
        datos = fopen(name, "r");
        if(datos==NULL) {
            printf("Missin file %s...\nExiting\n\n", name);
            exit(0);
        }

        for(i=1; i<=size; i++){
                for (j=1; j<=ncols; j++) {
                        fscanf(datos, "%lg", &datos1);
                        if(j==chosen_col) vector[i]=datos1;
                }
        }
        fclose(datos);
        return;
}



//---------------------------------------
void vector_to_file(char *name, double vector[], int start, int end)
{   
        int i;
        FILE *datos;
        datos = fopen(name, "w"); 
        for(i=start; i<=end; i++) fprintf(datos, "%lg\n", vector[i]);
        fclose(datos);
        return;
}   


void ivector_to_file(char *name, int vector[], int size)
{  
        int i;
        FILE *datos;
        datos = fopen(name, "w");
        for(i=1; i<=size; i++) fprintf(datos, "%d\n", vector[i]);
        fclose(datos);
        return;
}  
	

