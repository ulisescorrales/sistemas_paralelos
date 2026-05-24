//gcc -o punto11 punto11.c -fopenmp
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <unistd.h>

double sampleTime() {
   struct timespec tv;
   clock_gettime(CLOCK_MONOTONIC_RAW, &tv);
   return ((double)tv.tv_sec+((double)tv.tv_nsec)/1000000000.0);
}

int primo(int n) {
   int i,res,mitad;
   i = 2;
   res = 1;
   mitad = n / 2;
   while(res != 0 && i <= mitad) {
      res = n % i;
    	 i++;
   }  
   return res;
}

int main(int argc,char *argv[]){
	if(argc!=3){
		printf("Debe agregar inicio y fin como argumentos\n");
		exit(1);
	}
	int inicio=atoi(argv[1]);
	int fin=atoi(argv[2]);

	int i;
	int res;

	int diferencia=fin-inicio+1;
	
	#pragma omp parallel private(i,res)
	{
		double inicioTime=sampleTime();
		printf("Comienza thread %d\n",omp_get_thread_num());
		int cant=0;
		int cantLoops=0;
		#pragma omp for schedule(dynamic,1)
		for(i=inicio;i<=fin;i++){
			cantLoops++;
			res=primo(i);
			if(res!=0){
				cant++;
			}
		}
		double finTime=sampleTime();
		printf("Thread Nº %d registró %d primos, con %d loops en %f segundos\n",omp_get_thread_num(),cant,cantLoops,finTime-inicioTime);
	}
}
