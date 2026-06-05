// mpicc -o punto13 punto13.c
#include </usr/lib/x86_64-linux-gnu/openmpi/include/mpi.h>
#include <stdio.h>
#include <stdlib.h>                                                                                                                                                                    
#include <string.h>
int n=500000000;
void imprimir(int *arr,int cant_procesos,int irank){
	printf("cant procesos: %d\n",cant_procesos);
	for(int i=0;i<cant_procesos;i++){
		printf("Proceso %d: A[%d]\n",irank,arr[i]);
	}
}
int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);
	
	MPI_Comm cart_comm;
	const int dims[2]={3,5};
	const int periods[2]={1,1};
	MPI_Cart_create(MPI_COMM_WORLD,2,dims,periods,0,&cart_comm);
	printf("Init\n");
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	int arriba, abajo, derecha, izquierda;
	MPI_Cart_shift(cart_comm, 0, 1, &arriba, &abajo);
	MPI_Cart_shift(cart_comm, 1, 1, &izquierda, &derecha);

	printf("proceso [%d]arriba: %d, abajo: %d, izquierda: %d, derecha: %d\n",rank,arriba,abajo,izquierda,derecha);
	MPI_Finalize();
}
