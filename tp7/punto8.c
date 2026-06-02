// mpicc -o punto8 punto8.c
#include </usr/lib/x86_64-linux-gnu/openmpi/include/mpi.h>
#include <stdio.h>
#define MSGLEN 2048

int main(int argc, char *argv[]) {
	int ITAG_A = 100,ITAG_B = 200;
	int irank, i, idest, isrc, istag, iretag;
	float rmsg1[MSGLEN];
	float rmsg2[MSGLEN];
	MPI_Status recv_status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &irank);

	for (i = 0; i < MSGLEN; i++) {
		rmsg1[i] = 100;
		rmsg2[i] = -100;
	}
	if ( irank == 0 ) {
		idest  = 1;
		isrc   = 1;
		istag  = ITAG_A;
		iretag = ITAG_B;
	}
	else if ( irank == 1 ) {
		idest  = 0;
		isrc   = 0;
		istag  = ITAG_B;
		iretag = ITAG_A;
	}
	float buffer[MSGLEN];
	printf("La tarea %d va a enviar un mensaje\n", irank);
	//El buffer debe tener espacio para ambos mensajes
	//Se reemplaza Ssend por Bsend
	MPI_Request request;
	MPI_Isend(&rmsg1,MSGLEN , MPI_FLOAT, idest, istag, MPI_COMM_WORLD,&request);
	printf("Ha enviado un mensaje %d\n",irank);

	MPI_Wait(&request,MPI_STATUS_IGNORE);
	MPI_Recv(&rmsg2, MSGLEN, MPI_FLOAT, isrc, iretag, MPI_COMM_WORLD, &recv_status);
	printf("La tarea %d ha recibido el mensaje: %f\n", irank,rmsg2[MSGLEN-1]);
	MPI_Finalize();
}
