#include"convlv.c"
#include"savgol.c"
#include"realft.c"
#include"twofft.c"
#include"four1.c"
#include"lubksb.c"
#include"ludcmp.c"


int nearpow2(int number)
{
 	int i=0, temp=1;
	while(temp<number) {temp*=2; i++;}
        return(i-1);
}


int savitsky(double *input, int size, double *output, int np, int nl, int nr, int ld, int m)
{
	int index,i;
	int POT=nearpow2(size);
	float c[POT+1],data[POT+1],ans[2*POT+1],inp[size],dum;
        savgol(c,np,nl,nr,ld,m);	

	for(index=1;index<=size;index++) inp[index]=(float) input[index];
        

	for(index=1;index<=POT;index++) inp[index]=fabs(inp[index]);
	convlv(inp,POT,c,np,1,ans);
	
	for(index=2;index<POT-1;index++) output[index]=ans[index];
    	return 0;
}

