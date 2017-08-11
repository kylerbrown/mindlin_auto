// Automatic extraction of GTEs as LOCAL MINIMA, GLOBAL MAXIMA AND SYLLABLE BEGINNINGS AND ENDS
// Compile using: gcc gtes_example.c -lm -o gtes_example
// Run using: ./gtes_example

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "librerias.c"
#include "nrutil.h"
#include "nrutil.c"
#include "ht.c"
#include "rk4.c"

#include"convlv.c"
#include"savgol.c"
#include"realft.c"
#include"twofft.c"
#include"four1.c"
#include"lubksb.c"
#include"ludcmp.c"


struct Par {
    double beta; double tau;
} aa;


void takens(int n,double v[],double dv[],double t) {
    double x;    
    x=v[0];     
    dv[0]=-(1/aa.tau)*x+fabs(aa.beta);
    return;
}

int nearpow2(int number){
 	int i=0, temp=1;
	while(temp<number) {temp*=2; i++;}
        return(temp/2);
}



int main(int argc, char *argv[]) {
	char *asciifile, *csvfile;
	if (argc < 2){
		printf("specify ASCII data file as an argument, and a CSV file for output");
		return 1;
	}
	asciifile = argv[1];
	csvfile = argv[2];
	FILE *gteptr;
	gteptr = fopen(csvfile, "w");
   int i,j,k,Ndatos;

    //Loads song
    double *original;
    Ndatos=filesize(asciifile,1);
    original=dvector(1,Ndatos); 
    file_to_vector(asciifile,original,1,Ndatos,1,1);
    double *sound;
    sound=dvector(1,Ndatos);
    
    for(i=1;i<=Ndatos;i++) sound[i]=original[i];       
    //vector_to_file("song.exampleGTE.dat",sound,1,Ndatos);


    printf("\n\n\tNdatos %d\n\tClosest power of 2 %d\n\n",Ndatos,nearpow2(Ndatos));

   //Computes envelope using Hilbert Transform
   double *hilb;
   hilb=dvector(1,Ndatos);
   hilbert(sound,hilb,Ndatos); 
  // vector_to_file("hilbert.exampleGTE.dat",hilb,1,Ndatos);


   //Smoothes envelope through integration
   double v[1],dt, t;
   double *av_sound;
   av_sound=dvector(1,Ndatos);

   k=0;
   dt=1/10000.;
   aa.tau=0.001;
   for(i=0;i<Ndatos;i++){
        aa.beta=hilb[i];
        rk4(takens,v,1,t+0.0,dt);
        av_sound[i]=v[0];
   }
   //vector_to_file("envelope.exampleGTE.dat",av_sound,1,Ndatos);


   //Savitzky-Golay filtering
   int np,nl,nr,ld,m,index;
   int POT=nearpow2(Ndatos);
   float c[POT+1],data[POT+1],ans[2*POT+1],dum;
   double *sav;
   sav=dvector(1,Ndatos);   

   savgol(c,513,256,256,0,4);	
   for(i=1;i<=POT;i++) data[i]=(float) av_sound[i]; 
   for(index=1;index<=POT;index++) data[index]=fabs(data[index]);

   convlv(data,POT,c,513,1,ans);

   for(i=2;i<POT-1;i++) sav[i]=(double) ans[i]; 

   double *deri;
   deri=dvector(1,Ndatos);
   for(i=3;i<POT-1;i++) deri[i]=-sav[i+2]+8.*sav[i+1]-8.*sav[i-1]+sav[i-2];

   savgol(c,513,256,256,0,4);		
   for(i=1;i<=POT;i++) data[i]=(float) deri[i]; 
   convlv(data,POT,c,514,1,ans);
   for(i=2;i<POT-1;i++) deri[i]=(double) ans[i]; 
   

   //GTEs
   int temp=0, count=0, *inic, *fin;
   double *gte1=dvector(1,1000);
   double *gte2=dvector(1,1000);
   double *gte3a=dvector(1,1000);
   double *gte3b=dvector(1,1000);
   double *gte3=dvector(1,1000);
   double *gte4=dvector(1,1000); 
   double *gte5=dvector(1,1000); 
   
    double *norm; 
   norm=dvector(1,Ndatos);
   normalize(sav, Ndatos, norm);
   inic=ivector(1,100);
   fin=ivector(1,100);
   fputs("type,time\n", gteptr);	
   //Syllable beginnings and ends        
   for(i=1;i<POT;i++){ 
       if(norm[i]<0.025 && norm[i+1]>0.025) {
	       gte1[++temp]=(double) i;
	       inic[++count]=i;
	       fprintf(gteptr, "onset,%lg\n", i / 40e3);
       }
       if(norm[i]>0.025 && norm[i+1]<0.025) {
	       gte1[++temp]=(double) i;
	       fin[count]=i;
	       fprintf(gteptr, "offset,%lg\n", i / 40e3);
       }
   }	

    temp=0;
   //Syllable maximum
   for(i=1;i<=count;i++){
	   gte2[++temp]=(double) index_maximo(norm,inic[i],fin[i]);
	   fprintf(gteptr, "glob_max,%lg\n", gte2[temp] / 40e3);
   }



   //Intrasyllabic minima
    int comp;        
    temp=0;
    for(i=51;i<POT-52;i++) {        
        for(j=1;j<=count;j++){
            if(i>=inic[j]&&i<=fin[j])
            if(deri[i]<0. && deri[i+1]>0.)	
            gte3a[++temp]=(double) i; 
	}
    }
    comp=temp;

   //Intrasyllabic maxima       
    temp=0;
    for(i=51;i<POT-52;i++) {        
        for(j=1;j<=count;j++){
            if(i>=inic[j]&&i<=fin[j])
            if(deri[i]>0. && deri[i+1]<0.) 	
            gte3b[++temp]=(double) i; 
	}
    }    
    if(temp>comp) comp=temp;

    //Selects minima
    int min_loc, max_ant, max_post;
    temp=0;
    for(i=2;i<=comp;i++) {
	min_loc = (int) gte3a[i];
        max_ant=0; max_post=0;
	for(j=1;j<=comp;j++){
		if(gte3b[j]<gte3a[i] && gte3b[j+1]>gte3a[i]) {
			max_ant=(int) gte3b[j];
			max_post=(int) gte3b[j+1];}
	}
	if(max_ant && max_post) 
    	//----------------------------------------------------------------
        if(norm[min_loc]<0.8*norm[max_ant]){
		gte3[++temp]=gte3a[i];
	   	fprintf(gteptr, "sig_min,%lg\n", gte3[temp] / 40e3);
	}

    }

    //Selects maxima
    int max_loc, min_ant, min_post;
    temp=0;
    for(i=1;i<=comp;i++) {
        max_loc = (int) gte3b[i];
        min_ant=0; min_post=0;
        for(j=1;j<=comp;j++){
            if(gte3a[j]<gte3b[i] && gte3a[j+1]>gte3b[i]) {min_ant=(int) gte3a[j];min_post=(int) gte3a[j+1];}
        }
        if(min_ant || min_post) 
            //----------------------------------------------------------------
             if(norm[max_loc]>2.6*norm[min_ant] || norm[max_loc]>2.6*norm[min_post]) gte5[++temp]=gte3b[i];
    }

    //Last maximum
    temp=0;
    int mintemp;
    for(i=1;i<=count;i++){
        mintemp=inic[i];
        for(j=inic[i];j<=fin[i];j++){
            if((deri[j]>0)&&(deri[j+1]<0))mintemp=j;
        }
        gte4[++temp]=(double)mintemp;
	fprintf(gteptr, "last_max,%lg\n", gte4[temp] / 40e3);
    }

    
    vector_to_file("gtes1.exampleGTE.dat",gte1,1,1000);
    vector_to_file("gtes2.exampleGTE.dat",gte2,1,1000);
    vector_to_file("gtes4.exampleGTE.dat",gte4,1,1000);
    vector_to_file("gtes3.exampleGTE.dat",gte3,1,1000);
    
    free_ivector(inic,1,100);	
    free_ivector(fin,1,100);	
    free_dvector(gte1,1,1000);
    free_dvector(gte2,1,1000);
    free_dvector(gte3a,1,1000);
    free_dvector(gte3b,1,1000);
    free_dvector(gte3,1,1000);
    free_dvector(gte4,1,1000);
   
    free_dvector(norm,1,Ndatos);	
    free_dvector(sound,1,Ndatos);
    free_dvector(av_sound,1,Ndatos);
    free_dvector(hilb,1,Ndatos);
    free_dvector(sav,1,Ndatos);
    free_dvector(deri,1,Ndatos);

}

