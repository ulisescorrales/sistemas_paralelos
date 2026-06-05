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
	int n=10;

	MPI_Datatype mi_tipo;
	//stride= tamanio de la fila
	//count= cantidad de filas
	//block=datos de cada fila (1 en este caso)
	MPI_Type_vector(n, 1,n , MPI_INT, &mi_tipo);
	MPI_Type_commit(&mi_tipo);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if(rank==0){
		int matriz[n][n];
		int temp=0;
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				matriz[i][j]=temp;
				temp++;
			}
		}
		//count es la cantidad de datos de este tipo (solo uno en este caso)
		MPI_Send(matriz,1,mi_tipo,1,0,MPI_COMM_WORLD);
	}else if (rank==1){
		int buffer_recv[n];
		//Count es la cantidad de datos esperada
		MPI_Recv(buffer_recv,n,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		for(int i=0;i<n;i++){
			printf("proccess %d: %d\n",rank,buffer_recv[i]);
		}
	}

	MPI_Finalize();
}
