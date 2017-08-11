// .WAV to ASCII converter
#define N 1048576
#define PI 3.14159524288
#define RNG random()/2147483648.0

#include <math.h>
#include <time.h>
#include <stdio.h>


main(int argc, char *argv[]){
  short *ti;
  char temp[100];
  long i,ni;
  int max = 0,min=30000;
  FILE *fp;
  
  if(argc < 2){
    printf("You must give input soundfile \n");
    exit(-1);
  }
		

  sprintf(temp,"%s",argv[1]);
  fp=fopen(temp,"r");
  fseek(fp,0,SEEK_END);

  ni = ftell(fp)/sizeof(short);
  rewind(fp);

  ti = (short *)(malloc(ni*sizeof(short)));
  
  fread(ti,sizeof(short),N,fp);
  fclose(fp);
   
  for(i=22;i<ni;i++) 
    printf("%d\n",ti[i]);
}
