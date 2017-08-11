// FF trace smoother. Compile using: gcc smoothFF.c -lm -o smoothFF
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define COLsal 3

int filesize(char name[], int ncol);

int main(int argc, char *argv[]){
  int size;
  
  char *entrada;


if(argc < 2){
    printf("\nUsage:\n ./smoothFF [Inputfile], where Inputfile is the output of compute_FF\n");
    return 1;
  }

 
 entrada = argv[1];
 size =filesize(entrada,COLsal)-1; 

double tiempot[size]; double frec[size];

 int i,j,ii, contador, contadorprimeros;
 
  double tiempo, nada,sumafrec;
  double v[2];
i=0; contadorprimeros=0;
 FILE *ptr1;

ptr1=fopen(entrada,"r");

       
 
             
  while ((i<size)){
fscanf(ptr1,"%lg %lg %lg",&nada,&tiempot[i],&frec[i]);  
i++;             
}
i=0;
 for (i=0;i<size-350;i++){

if((frec[i]==0)&&(frec[i+1]==0)&&(frec[i-1]>0)){
for(j=0;j<350;j++) {frec[i+j] = 0;frec[i-j] = 0;}
   }
}

for(i=0;i<size-100;i++){ 

contador = 1;
sumafrec=0;

for(j=0;j<100;j++){    
  if(frec[i]>0) {sumafrec=sumafrec+frec[i+j]; if(frec[i+j]>0)contador++;}
}


frec[i]=sumafrec/(contador*1.);
printf("%d\t%lg\t%lg\n",i,tiempot[i],frec[i]); 
    
}
}
//------------------------------------------------------------------//

int filesize(char name[], int ncol){

  int i, size = 1;
  char dato[20];
  FILE *datos;
  
  datos = fopen(name, "r");
  
  if(datos==NULL) {     
    printf("File missing %s...\nExiting to system\n\n", name);
    exit(0);
  }
  
  while(!feof(datos)){
    for(i=1; i<=ncol; i++) fscanf(datos, "%s", &dato);
    size++;		
  }
  
  fclose(datos);
  return(size-1);
}

