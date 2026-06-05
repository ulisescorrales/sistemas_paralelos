//gcc -o punto15 punto15.c -fopenmp
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#define N 200000000
void vector_add(int *A, int *B, int *C, int n) {
	int i;
	for (i = 0; i < n; i++)
		C[i] = A[i] + B[i];
}

void rec_vector_add(int *A, int *B, int *C, int n) {
	// si es el caso base, llamar a vector_add
	// sino, hacer la suma recursiva sobre la primera mitad de los arreglos, y luego la suma recursiva sobre la segunda mitad de los arreglos.
	/* printf("Soy thread %d\n",omp_get; */
	if(n<=100000){
		vector_add(A,B,C,n);
	}else{
		int mitad=n/2;
		#pragma omp task
		{
			/* printf("Soy thread %d\n",omp_get_thread_num()); */
			rec_vector_add(A,B,C,mitad);
		}
			/* vector_add(A,B,C,mitad); */
		int i;
		for (i = mitad; i < n; i++)
			C[i] = A[i] + B[i];
		}
}

int main() {
	int *a=malloc(N*sizeof(int));
	int *b=malloc(N*sizeof(int));
	int *c=malloc(N*sizeof(int));
	printf("Inicio\n");
	for(int i=0;i<N;i++){
		a[i]=i;
		b[i]=i;
		c[i]=i;
	}	
	#pragma omp parallel
	{
		#pragma omp single
		{
			rec_vector_add(a, b, c, N);
		}
	}

	for(int i=0;i<10;i++){
		printf("%d\n",c[i]);
	}
}

