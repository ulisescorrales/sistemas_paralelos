//mpicc -o punto5 punto5.c
#include <string.h>
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
	int cant_procesos;
	MPI_Comm_size(MPI_COMM_WORLD,&cant_procesos);
	if(rank==cant_procesos-1){
		//Recibir el mensaje e imprimirlo
		char buff[100];
		MPI_Recv(&buff,100,MPI_BYTE,cant_procesos-2,MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		printf("Recibido el mensaje %s desde el proceso %d\n",buff,rank);
	}else if(rank!=0){
		char buff[100];
		MPI_Recv(&buff,100,MPI_BYTE,rank-1,MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		int count=strlen(buff)+1;
		printf("Recibido el mensaje %s desde el proceso %d\n",buff,rank);
		MPI_Send(&buff,count,MPI_BYTE,rank+1,0,MPI_COMM_WORLD);
	}else if(rank==0){
		char *mensaje="holaaaa";
		int count=strlen(mensaje)+1;
		printf("len mensaje: %d\n",count);
		printf("Enviar mensaje: %s\n",mensaje);
		MPI_Send(mensaje,count,MPI_BYTE,rank+1,0,MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}
