/* gcc punto5_d.c -o punto5_d -fopenmp */
#include <string.h>
#include <omp.h>
#include <stdio.h>
void main () {
   long num_steps = 800000000;
   double step;
   int i;
   double x, pi=0.0;
   step = 1.0/(double) num_steps;

	int cant_threads=omp_get_max_threads();
	double sum=0.0;
	printf("max cant threads: %d\n",cant_threads);
	memset(&sum,0,sizeof(sum));

	omp_set_dynamic(1);

	double sumaTotal=0.0;
	#pragma omp parallel private(x,sum)
	{
		printf("Arranca thread Nº %d\n",omp_get_thread_num());
		int cant_threads=omp_get_num_threads();
		int porcion_step=num_steps/cant_threads;
		int num_thread=omp_get_thread_num();
		long step_ini=porcion_step*num_thread;
		long step_final=step_ini+porcion_step;
		for (int i=step_ini;i<step_final;i++){
      		x = (i + 0.5) * step;
			sum+=4.0 / (1.0 + x * x);
		}
		#pragma omp critical
		sumaTotal+=sum;
	}

   pi = step * sumaTotal;
   printf("%f\n",pi);
}

