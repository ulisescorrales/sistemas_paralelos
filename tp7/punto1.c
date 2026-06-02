//mpicc punto1.c -o punto1
//mpirun -n 8 --oversubscribe punto1
//instalar MPI en debian: https://unix.stackexchange.com/questions/237670/mpi-h-not-found
#include <stdlib.h>
#include <stdio.h>
#include </usr/lib/x86_64-linux-gnu/openmpi/include/mpi.h>

int main( int argc, char *argv[]) {
	// iniciar entorno MPI
	int rank;
	char *name=malloc(sizeof(char)*MPI_MAX_PROCESSOR_NAME);
	int len;
	int cantidad;

	MPI_Init(&argc,&argv);
	printf("Hola mundo!\n");
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	printf("Rank: %d\n",rank);
	MPI_Get_processor_name(name,&len);
	printf("processor: %s\n",name);
	
	MPI_Comm_size(MPI_COMM_WORLD,&cantidad);
	printf("cant: %d\n",cantidad);
	MPI_Finalize();
	return 0;
}

