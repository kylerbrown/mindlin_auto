#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rk4.c"


#define c 35000
struct Par {
                    double tau; double beta;
} aa;


void takens(int n,double v[],double dv[],double t) {
        extern double PRESSURE,presion,acople;
        double x;
      
x=v[0]; 


 dv[0]=-(1/aa.tau)*x+fabs(aa.beta);

        return;
}




void integrando(int size, char *entrada, float *envolvente){
  int i;
  int t,to;
  double tiempot;
  double dt,x ;
  double v[1];
  char FileName[19];
 FILE *ptr1,*ptr2;
 

aa.tau=0.001;
ptr1=fopen(entrada,"r");

    sprintf(FileName,"env_%s", entrada);
    ptr2=fopen(FileName, "w");


	
	v[0]=.00;
	tiempot=0.;
        dt=1./(44100.0);
t=0;
  to=size;
                 
  while ((t<to)){
tiempot=tiempot+dt;

fscanf(ptr1,"%lg",&aa.beta);

rk4(takens,v,1,t+0.0,dt);
x=(v[0]-0.2*tiempot/(0.665));

envolvente [t] =v[0];

fprintf(ptr2,"%lg\t%lg\t%lg\n",t*(1/44100.),aa.beta,v[0]);
t++;             
}

  }
