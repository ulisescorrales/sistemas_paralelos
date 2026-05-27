// gcc -o -03 -lm punto12 punto12.c -fopenmp
#include <stdio.h>
#include <math.h>
#define N 1000
#define PASOS 1000

int main()
{
   double A[N][N], b[N], x[N], x2[N];
   double max=-1.0, q=0, s, e;
   int i, j, k, n=N, pasos=PASOS;


   //inicialización
   for (i=0;i<n;i++)
      b[i] = 0.5;
   for (i=0;i<n;i++)
      x[i] = 0.5+(double)i/n;
   for (i=0;i<n;i++)
      for (j=0;j<n;j++)
         A[i][j] = 0.6*(sin(i+j)+1)/2;
   //fin inicialización


   //q=1;
   //b[i] siempre vale 0.5
   for (k=0;k<pasos;k++) {
      q=1;
	  //for1
      for (i=0;i<n;i++) {
         s = b[i];
         for (j=0;j<n;j++) {
            e = sin(A[i][j] * x[j]);
            s+= e * e * e + 2 * e * e + 5 * e;
         }
         x2[i] = s;
         q *= sin(s);
      }
	  //No puede seguir si no termina este for por la dependencia con x2 y x1
	  //for2
      for (i=0;i<n;i++)
         x[i] = x2[i];
	  //puede ejecutarse indistintamente que el anterior for (usar nowait)
	  //Debe ejecutarse después de for1
	  //for3
      for (i=0;i<n/2;i++)
         b[i] = b[i] + 0.00002;

	  //max y q nunca cambian
	  //puede ejecutarse indistintamente al for más grande
      if (max<q) {
         max = q;
         printf("max: %.10e\n", max);
      }
   }
   printf("max final: %.10e\n", max);
   return max;
}

