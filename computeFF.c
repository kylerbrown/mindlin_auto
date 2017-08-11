// Compile using: gcc compute_FF.c -lm -o compute_FF realft.c four1.c nrutil.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "integrando_void.c" 

#define N_NAME1_AUX 50

//-----------------------------------------------------------------


int main(int argc, char *argv[]){

  int size, filtro, pow;
  int largo;
  int largosobre2;
  char *entrada;

if(argc < 3){
    printf("\nUsage:\n ./compute_FF [File length] [File Name]\n");
    return 1;
  }


 size = atoi(argv[1]);
 entrada = argv[2];
 
 largosobre2 = 1024;
 largo = 2 * largosobre2;
   
// FFT parameters and computation

    int m,h,i,j,k,l,ikp, pasos, sample_max;
    double sampling = 44100.0;
    float data[largo], datareal[largosobre2];
    float dataux[largo];
    float power[largo];
    float frecuencia[largosobre2];
    float serie_fft[largosobre2];
    float serie_temp[size];
    float envuelve[size];
    float sigma=5./1000.*sampling, gaussiana[largosobre2];
    float datareal_filtrada[largosobre2], datareal_filtrada_ant[largosobre2];
    float nada;
    
    
 int contadormax[size];
 float maximo[largosobre2], frec_max[largosobre2],max, freq;

FILE *ptr;
FILE *ptr1;
FILE *ptr2;
FILE *ptr3;
FILE *ptr4;


ptr=fopen(entrada,"r");


 for(m=0;m<size;m++){
        fscanf(ptr,"%f ",&serie_temp[m]);
	}

fclose(ptr);    


integrando(size, entrada, envuelve);
    
for(l=0;l<largosobre2*0.5;l++)   printf("%d\t%lg\t%f\n",l,l/44100.,0.);

for(l=largosobre2*0.5;l<size-largosobre2*0.5;l++){ 

 for(m=-largosobre2*0.5;m<largosobre2*0.5;m++) {
     datareal[m+1] = serie_temp[m+1*l];
     gaussiana[m+1] = exp(-(m+1)*(m+1)/(sigma*sigma));  
     datareal_filtrada_ant[m+1] = datareal[m+1]*gaussiana[m+1];    
     }
   
for(m=0;m<largosobre2;m++) {datareal_filtrada [m+1] =datareal_filtrada_ant[m+1-512];}
  
     
    int isign=1;
    
    realft(datareal_filtrada, largosobre2,isign);

//--------------------------------------------------------------------------

j=1;
k=0;    
power[1]= datareal_filtrada[1];
power[largosobre2] = datareal_filtrada[2];



for(i=3;i<largosobre2;i+=2){
power[i]=datareal_filtrada[2*i]*datareal_filtrada[2*i]+datareal_filtrada[2*i+1]*datareal_filtrada[2*i+1];}

for(i=1;i<largosobre2;i+=2) {

    serie_fft[k] = sqrt(power[i]);	
    frecuencia[k] = (i/(1.*largosobre2))*sampling;

	k++;		
 	
}
    
contadormax[l]=0;
    
for(i=1;i<largosobre2;i++){

  if((serie_fft[i]>serie_fft[i+1])&&(serie_fft[i]>serie_fft[i-1])&&(frecuencia[i]>450.)&&(frecuencia[i]<7000.)&&(serie_fft[i]>12000.)){

   frec_max[contadormax[l]] = frecuencia[i]; 
   contadormax[l]++;
   }

  else{frec_max[contadormax[l]]= 0.;}
}

   printf("%d\t%lg\t%f\n",l,(l)/44100.,frec_max[0]);
  

  }

return 0;
}
