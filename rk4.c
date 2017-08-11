/* Runge Kutta integrator from numerical recipies plus improvements */
/* void *deri(int n,double h[],double D[],double t);  */
/* function argument not tested yet */

void rk4(void deri(int , double [], double [], double ), \
double h[], int n, double t, double dt)
{
#define naux 260 

int i;
double k1[naux],k2[naux],k3[naux],k4[naux],h0[naux];
double dt2, dt6;

dt2=dt/2.;
dt6=dt/6.;

for (i = 0 ; i<n; i++)
	h0[i] = h[i];

deri(n,h0,k1,t);
for (i =0 ; i<n ; i++)
	h0[i]=h[i]+dt2*k1[i];

deri(n,h0,k2,t+dt2);
for (i =0 ; i<n ; i++)
	h0[i]=h[i]+dt2*k2[i];

deri(n,h0,k3,t+dt2);
for (i =0 ; i<n ; i++)
	h0[i]=h[i]+dt*k3[i];

deri(n,h0,k4,t+dt);
for (i = 0 ; i<n ; i++)
	{h0[i]=h[i]+dt*k4[i];};

for (i =0; i<n ; i++)
	h[i]=h[i]+dt6*(2.*(k2[i]+k3[i])+k1[i]+k4[i]);

return;
}

