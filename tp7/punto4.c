//mpicc -o punto4 punto4.c
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

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	if(rank==0){
	}else if(rank==1){

	}

	MPI_Finalize();
	return 0;
}
