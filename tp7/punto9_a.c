// mpicc -o punto9_a punto9_a.c
#include </usr/lib/x86_64-linux-gnu/openmpi/include/mpi.h>
#include <stdio.h>
#include <stdlib.h>                                                                                                                                                                    
#include <string.h>
int n=500000000;
void imprimir(int *arr,int irank){
	for(int i=0;i<n;i++){
		printf("Proceso %d: A[%d]\n",irank,arr[i]);
	}
}
int main(int argc, char *argv[]) {
	int *A=malloc(sizeof(int)*n);
	int *buffer_recv=malloc(sizeof(int)*n);
	MPI_Init(&argc, &argv);

	int irank;
	MPI_Comm_rank(MPI_COMM_WORLD, &irank);

	for(int i=0;i<n;i++){
		A[i]=i+irank;
	}
	
	int *B=malloc(sizeof(int)*n);
	int size;
    MPI_Pack_size(n, MPI_INT, MPI_COMM_WORLD, &size);
	printf("%d\n",size);
	int buffer_size=size+MPI_BSEND_OVERHEAD;
	int *attach=malloc(buffer_size);
	MPI_Buffer_attach(attach,buffer_size);
	printf("Proceso %d: init\n",irank);
	if ( irank == 0 ) {
		MPI_Bsend(A,n,MPI_INT,1,0,MPI_COMM_WORLD);
		MPI_Recv(buffer_recv,n,MPI_INT,1,MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	}
	else if ( irank == 1 ) {
		MPI_Bsend(A,n,MPI_INT,0,0,MPI_COMM_WORLD);
		MPI_Recv(buffer_recv,n,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	}
	printf("Proceso %d: mensaje recibido!\n",irank);

	int X=0;
	for(int i=0;i<n;i++){
		X+=A[i];
	}
	for(int i=0;i<n;i++){
		A[i]=X+buffer_recv[i];
	}
	printf("Proceso %d: X es igual a %d\n",irank,X);
	
	double seconds=	MPI_Wtime();
	printf("Time proceso %d: %f segundos\n",irank,seconds);
	MPI_Finalize();
}
