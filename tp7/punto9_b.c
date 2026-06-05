// mpicc -o punto9_b punto9_b.c
#include </usr/lib/x86_64-linux-gnu/openmpi/include/mpi.h>
#include <stdio.h>
#include <stdlib.h>                                                                                                                                                                    
#include <string.h>
int n=5000000000;
void imprimir(int *arr,int irank){
	for(int i=0;i<n;i++){
		printf("Proceso %d: A[%d]\n",irank,arr[i]);
	}
}
int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);

	int *A=malloc(sizeof(int)*n);
	int *buffer_recv=malloc(sizeof(int)*n);
	int irank;
	MPI_Comm_rank(MPI_COMM_WORLD, &irank);

	for(int i=0;i<n;i++){
		A[i]=i+irank;
	}

	int *B=malloc(sizeof(int)*n);
	MPI_Request request_send;
	MPI_Request request_recv;
	if ( irank == 0 ) {
		MPI_Isend(A,n,MPI_INT,1,0,MPI_COMM_WORLD,&request_send);
		MPI_Irecv(buffer_recv,n,MPI_INT,1,MPI_ANY_TAG,MPI_COMM_WORLD,&request_recv);
	}
	else if ( irank == 1 ) {
		MPI_Isend(A,n,MPI_INT,0,0,MPI_COMM_WORLD,&request_send);
		MPI_Irecv(buffer_recv,n,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&request_recv);
	}

	int X=0;
	for(int i=0;i<n;i++){
		X+=A[i];
	}
	printf("Proceso %d: init\n",irank);
	MPI_Wait(&request_recv,MPI_STATUS_IGNORE);
	for(int i=0;i<n;i++){
		A[i]=X+buffer_recv[i];
	}
	printf("Proceso %d: X es igual a %d\n",irank,X);
	double seconds=	MPI_Wtime();
	printf("Time proceso %d: %f segundos\n",irank,seconds);
	MPI_Finalize();
}
