//gcc punto6.c -o punto6 -fopenmp
#include <time.h>
#include <omp.h>
#include <stdio.h>
#include <unistd.h>
double sampleTime() {
   struct timespec tv;
   clock_gettime(CLOCK_MONOTONIC_RAW, &tv);
   return ((double)tv.tv_sec+((double)tv.tv_nsec)/1000000000.0);
}
void trabajo(int id){
	printf("Comienza el thread Nº %d\n",omp_get_thread_num());
    printf("%d: Trabajo muy duro...\n", id);
    sleep(1);
    printf("%d: como un esclavo!\n", id);
    sleep(1);
}

int main(){
	int i;
	// omp_set_dynamic(1);
	#pragma omp parallel for num_threads(8
	for(i=0;i<8;i=i+1){
		trabajo(i);
	}
    // trabajo(0);
    // trabajo(1);
    // trabajo(2);
    // trabajo(3);
    // trabajo(4);
    // trabajo(5);
    // trabajo(6);
    // trabajo(7);
    return 0;
}

