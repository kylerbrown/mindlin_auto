#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI2 6.2831853
#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)
#define Nforzante 100
#define Nforzados 100
#define N_NAME1_AUX 30
#define N_NAME2_AUX 30
#define S(x) 1/(1+exp(-(x/0.015)))

#define c 35000

struct Par {
		    double A1; double A2; double A3; 
		    double Ancho1; double Ancho2; double Ancho3;                 
		double r; double gamma2; double gamma3;
		double gm; double forcing1; double forcing2;
	       	double alfa1; double alfa2; double beta1; double beta2; double beta3; double alfa3;
		double RBoverLB; double s1overLG; double s1overLB; double s1overCH; double LGoverLB; double RB;
		double noise; double rdis; double amplitud;
} aa;


void takens(int n,double v[],double dv[],double t) {
        extern double PRESSURE,presion,acople;
        double x,y,i1,i2,i3,w2,alfa=50,x2,y2, x3, y3;
      
x=v[0]; y=v[1], i1=v[2], i2=v[3]; i3=v[4];x2=v[5],y2=v[6], w2=v[7];
x3=v[8]; y3=v[9];

 dv[0]=  y;
 dv[1]=  aa.alfa1*aa.gm*aa.gm+aa.beta1*aa.gm*aa.gm*(x)-aa.gm*aa.gm*x*x*x-aa.gm*x*x*y+aa.gm*aa.gm*x*x-aa.gm*x*y;

 dv[2]=i2; 
 dv[3]=-aa.s1overLG*aa.s1overCH*i1-aa.rdis*(aa.s1overLB+aa.s1overLG)*i2+i3*(aa.s1overLG*aa.s1overCH-aa.rdis*aa.RB*aa.s1overLG*aa.s1overLB)+(aa.s1overLG)*aa.forcing2+aa.rdis*aa.s1overLG*aa.s1overLB*aa.forcing1; 
 dv[4]=-(1/aa.s1overLG)*aa.s1overLB*i2-aa.RB*aa.s1overLB*i3+aa.s1overLB*aa.forcing1;  

 dv[5]= y2; 
 dv[6]= aa.alfa2*aa.gm*aa.gm+aa.beta2*aa.gm*aa.gm*(x2)-aa.gm*aa.gm*x2*x2*x2-aa.gm*x2*x2*y2+aa.gm*aa.gm*x2*x2-aa.gm*x2*y2;
 dv[7]= 0.; 
 
 dv[8]=  y3;
 dv[9]=  -1.*aa.gamma2*aa.gamma2*aa.alfa3*x3+aa.gamma2*aa.beta3*y3-aa.gamma3*aa.gamma2*2.0*(40)*(40)*x3*x3*y3;




        return;
}

float ran1(long *idum)
{
	int j;
	long k;
	static long iy=0;
	static long iv[NTAB];
	float temp;

	if (*idum <= 0 || !iy) {
		if (-(*idum) < 1) *idum=1;
		else *idum = -(*idum);
		for (j=NTAB+7;j>=0;j--) {
			k=(*idum)/IQ;
			*idum=IA*(*idum-k*IQ)-IR*k;
			if (*idum < 0) *idum += IM;
			if (j < NTAB) iv[j] = *idum;
		}
		iy=iv[0];
	}
	k=(*idum)/IQ;
	*idum=IA*(*idum-k*IQ)-IR*k;
	if (*idum < 0) *idum += IM;
	j=iy/NDIV;
	iy=iv[j];
	iv[j] = *idum;
	if ((temp=AM*iy) > RNMX) return RNMX;
	else return temp;
}



double PRESSURE=0.0; double presion, acople;


void finch(int size, char *salida, char *envolvente){

  long idum=(-1);
  int i,j,taux,iii,ik,ikp;
  int t,to,maxtau,tau1,tau2,tau3;
  double r12,r23,r21,r32,t12,t21,t23,t32,r34,t34,tiempot,tnoise,tcount;
  double *a,*bf,*bb,*cf,*cb,*df,*db;
  double ancho1,ancho2,ancho3,largo1,largo2,largo3;
  double dt, ddb,dbold,nada,atenua;
  double v[10];
  double preout;
  char FileName[19];


 FILE *ptr,*ptr1,*ptr2,*ptr3, *ptr4;
        char nombre_archivo1[N_NAME1_AUX];

for(ik=1;ik<2;ik++){
for(ikp=1;ikp<2;ikp++){


ptr2=fopen(salida,"r");
ptr3=fopen(envolvente,"r");

   sprintf(FileName,"song_%s", salida);
    ptr4=fopen(FileName, "w");



 v[0]=.000000000005;v[1]=0.00000000001;
 v[2]=.000000000001;v[3]=0.00000000001;
 v[4]=.000000000001;
 
v[5]=.0000000000045;v[6]=.000000000000001;

v[7]=0.;
v[8]=0.1;v[9]=0.0;

aa.forcing1=0.;
aa.forcing2=0.;
tiempot=0.;
tcount=0;
aa.noise=0;
tnoise=0;
aa.r=0.4;
aa.rdis=7000;
aa.gamma2=1.;
aa.gamma3=1.;
atenua=1;
       dt=1./(882000);

  to=size*20;

  a=(double*) calloc(to,sizeof(double));
  bf=(double*) calloc(to,sizeof(double));
  bb=(double*) calloc(to,sizeof(double));
  cf=(double*) calloc(to,sizeof(double));
  cb=(double*) calloc(to,sizeof(double));

  db=(double*) calloc(to,sizeof(double));
  df=(double*) calloc(to,sizeof(double));


aa.Ancho1=0.2; aa.Ancho2=.2; aa.Ancho3=0.2;
ancho1 = aa.Ancho1; ancho2 = aa.Ancho2; ancho3 = ancho2; largo1 = 1.5; largo2 =1.5;
 largo3 = 1.;




  r12=(ancho1-ancho2)/(ancho1+ancho2);r21=-r12;
  r23=(ancho2-ancho3)/(ancho2+ancho3);r32=-r23;
  t12=1+r12;t21=1+r21;
  t23=1+r23;t32=1+r32;



  tau1=rint(largo1/(c*dt+0.0));
  tau2=rint(largo2/(c*dt+0.0));
  tau3=rint(largo3/(c*dt+0.0));

 
  maxtau=tau3;
  if(tau1>=tau2&&tau1>=tau3) maxtau=tau1;
  if(tau2>=tau1&&tau2>=tau3) maxtau=tau2;
  t=0;

  for (j=0;j<=maxtau;j++) {
    bf[j]=0;bb[j]=0;cf[j]=0;cb[j]=0;df[j]=0;db[j]=0;
  }

  t=maxtau;
  taux=0;


aa.alfa2=.150;
aa.beta2=.150;
aa.alfa3=10000000;
aa.beta3=-200;

aa.s1overCH=(36*2.5*2/25.)*1e09;
aa.s1overLB=1.*1e-04;
aa.s1overLG=(50*1e-03)/.5;
aa.RB=1*1e07;


  while ((t<to)&&(v[1]>-5000000)){


    dbold=db[t];


    a[t]= (.50)* (1.01*(1.*(aa.A1*v[1]+aa.A2*v[6]+aa.A3*v[9]/10.)))+bb[t-tau1];
    bb[t]=r12*a[t-tau1]+t21*cb[t-tau2];
    bf[t]=t12*a[t-tau1]+r21*cb[t-tau2];
    cb[t]=r23*bf[t-tau2]+t32*db[t-tau3];
    cf[t]=t23*bf[t-tau2]+r32*db[t-tau3];
    db[t]=-aa.r*cf[t-tau3];

ddb=(db[t]-dbold)/dt;
aa.forcing1=db[t];
aa.forcing2=ddb;

PRESSURE=a[t];

tiempot=tiempot+dt;


aa.gm=24000.;

ancho1 = aa.Ancho1; ancho2 = aa.Ancho2;
r12=(ancho1-ancho2)/(ancho1+ancho2);r21=-r12;
  r23=(ancho2-ancho3)/(ancho2+ancho3);r32=-r23;
    t12=1+r12;t21=1+r21;
      t23=1+r23;t32=1+r32;


        rk4(takens,v,10,t+0.0,dt);
aa.noise=0;
preout=aa.RB*v[4];

if(taux==20) {
    
		fscanf(ptr2,"%lg %lg",&nada,&aa.beta1); 
		fscanf(ptr3,"%lg %lg %lg",&nada,&nada,&aa.amplitud);                
		fprintf(ptr4,"%lg\n",preout*10);
                 
		taux=0;
             }
    taux++;
    fflush(NULL);



if(tiempot>0.00){
 
aa.alfa1= -0.15-0.00*aa.amplitud;
aa.alfa2=0.15; aa.beta2=0.1; 
aa.r=0.1; 
aa.noise=.21*(ran1(&idum)-0.5);
aa.beta1= aa.beta1+0.01*aa.noise;
aa.s1overCH=(360/0.8)*1e08; 
aa.s1overLB=1.*1e-04; 
aa.s1overLG=(1/82.); 
aa.RB=(.5)*1e07;

aa.rdis=(300./5.)*(10000.);
aa.A1=(aa.amplitud)+0.5*aa.noise;
    if((tiempot>0.1)&&(tiempot<0.165)) aa.A1=sqrt(aa.amplitud)+10.5*aa.noise;
    if((tiempot>0.215)&&(tiempot<0.315)) aa.A1=sqrt(aa.amplitud)+10.5*aa.noise;
    if((tiempot>0.3425)&&(tiempot<0.499)) aa.A1=sqrt(aa.amplitud)+10.5*aa.noise;
    if((tiempot>0.214)&&(tiempot<0.255)) aa.A1=aa.A1/2.;
aa.A2=aa.amplitud*0.0;
aa.A3=0.;
}

    t++;

  }


   free(a);
   free(bf);
   free(bb);
   free(cf);
   free(cb);
   free(df);
   free(db);

}
}

}
