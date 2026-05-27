// gcc -o punto12_mod -O3 punto12_mod.c -fopenmp -lm
#include <stdio.h>
#include <math.h>
#include <omp.h>
#define N 1000
#define PASOS 1000

// max: -2.1072679683e-314
// max: -1.2194380051e-318
// max: 6.6657639047e-313
// max: 4.5382706763e-298
// max: 5.7131808138e-286
// max: 2.8396380000e-276
// max: 1.0684498673e-275
// max: 3.0333485841e-271
// max: 9.9805048351e-269
// max: 1.0248341479e-265
// max: 1.0099078918e-262
// max final: 1.0099078918e-262

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


	for (k=0;k<pasos;k++) {
		q=1;
		#pragma omp parallel
		{
		#pragma omp for reduction(*:q) private(j)
		for (i=0;i<n;i++) {
			s = b[i];
			for (j=0;j<n;j++) {
				e = sin(A[i][j] * x[j]);
				s+= e * e * e + 2 * e * e + 5 * e;
			}
			x2[i] = s;
			q *= sin(s);
		}
			#pragma omp for schedule(static) nowait
			for (i=0;i<n;i++)
				x[i] = x2[i];

			#pragma omp for schedule(static) 
			for (i=0;i<n/2;i++)
				b[i] = b[i] + 0.00002;
		}
			if (max<q) {
				max = q;
				printf("max: %.10e\n", max);
			}
	}
	printf("max final: %.10e\n", max);
	return max;
}
