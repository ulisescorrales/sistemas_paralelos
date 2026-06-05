// mpicc -o punto11 punto11.c
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

	int irank;

	int n=10;
	int cant_procesos;
	MPI_Comm_size(MPI_COMM_WORLD,&cant_procesos);
	int size_buffer=n*cant_procesos*sizeof(int);
	int *send_buffer;

	int cant_por_proceso=n;
	int recv_buffer[cant_por_proceso];
	MPI_Comm_rank(MPI_COMM_WORLD, &irank);

	int resultado_buffer[cant_por_proceso];

	if(irank==0){
		//Inicializar el buffer
		send_buffer=(int *)malloc(size_buffer);
		for(int i=0;i<n*cant_procesos;i++){
			send_buffer[i]=i;
		}
		/* printf("Arreglo inicializado:\n"); */
		/* imprimir(send_buffer,n*cant_procesos,irank); */
	}
	MPI_Scatter(send_buffer,cant_por_proceso,MPI_INT,
			 recv_buffer,cant_por_proceso,MPI_INT,0,MPI_COMM_WORLD);

	int sum=0;
	//Por cada proceso
	printf("Soy %d\n",irank);
	for(int i=0;i<cant_por_proceso;i++){
		sum+=recv_buffer[i];	
	}

	MPI_Barrier(MPI_COMM_WORLD);
	imprimir(recv_buffer,cant_por_proceso,irank);

	MPI_Gather(&sum,1,MPI_INT,resultado_buffer,1,MPI_INT,0,MPI_COMM_WORLD);

	if(irank==0){
		printf("Recibidos:\n");
		imprimir(resultado_buffer,cant_procesos,0);
	}

	MPI_Finalize();
}
