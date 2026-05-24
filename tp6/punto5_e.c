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

	omp_set_dynamic(1);

	#pragma omp parallel reduction(+:sum)
	{
		double x=0.0;
		printf("Se ejecuta thread Nº %d\n",omp_get_thread_num());
		int porcion_step=num_steps/omp_get_num_threads();
		int num_thread=omp_get_thread_num();
		long step_ini=porcion_step*num_thread;
		long step_final=step_ini+porcion_step;
		for (int i=step_ini;i<step_final;i++){
			sum=sum+(4.0 / (1.0 + x * x));
		}
	}
   pi = step * sum;
   printf("%f\n",pi);
}

