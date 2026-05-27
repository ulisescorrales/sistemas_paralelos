//mpicc -o punto3 punto3.c
#include </usr/lib/x86_64-linux-gnu/openmpi/include/mpi.h>
// #include <stdlib.h>
// #include <time.h>
#include <stdio.h>
int main(int argc,char *argv[]){
	int cant=11;
	int a[cant];
	for(int i=0;i<cant;i++){
		a[i]=i;
	}

	MPI_Init(&argc,&argv);

	int cantProcesos;
	MPI_Comm_size(MPI_COMM_WORLD,&cantProcesos);
	printf("Cant. procesos: %d\n",cantProcesos);
	printf("CantPorProceso: %d\n",cantProcesos);

	int cantPorProceso=cant/cantProcesos;

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	int limite=cantPorProceso*rank+cantPorProceso;
	for(int i=cantPorProceso*rank;i<limite;i++){
		printf("%d, proceso %d\n",a[i],rank);
	}
	MPI_Finalize();
	return 0;
}
