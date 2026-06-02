//            mpicc -o punto4 punto4.c
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

	float arr[]={1.00,2.00,3.00,10.00};
	MPI_Init(&argc,&argv);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	if(rank==0){
		MPI_Send(&arr,4,MPI_FLOAT,1,123,MPI_COMM_WORLD);
	}else if(rank==1){
		float buff[20];
		MPI_Recv(&buff,4,MPI_FLOAT,0,MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		printf("Mensaje recibido: %f\n",buff[3]);
	}

	MPI_Finalize();
	return 0;
}
