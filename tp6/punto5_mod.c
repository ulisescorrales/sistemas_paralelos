/* gcc punto5_mod.c -o punto5_mod -fopenmp */
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
	double sum[cant_threads];
	printf("max cant threads: %d\n",cant_threads);
	memset(&sum,0,sizeof(sum));

	omp_set_dynamic(cant_threads);

	#pragma omp parallel private(x)
	{
		int porcion_step=num_steps/omp_get_num_threads();
		int num_thread=omp_get_thread_num();
		long step_ini=porcion_step*num_thread;
		long step_final=step_ini+porcion_step;
		for (int i=step_ini;i<step_final;i++){
      		x = (i + 0.5) * step;
			sum[num_thread]+=4.0 / (1.0 + x * x);
		}
	}
	double sumTotal=0.0;
	int j=0;
	while(sum[j]!=0.0){
		sumTotal+=(double)sum[j];
		j++;
	}
   pi = step * sumTotal;
   printf("%f\n",pi);
}

