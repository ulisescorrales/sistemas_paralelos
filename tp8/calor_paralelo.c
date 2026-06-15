//mpicc -o calor_paralelo calor_paralelo.c -lm
#include <unistd.h> 
#include <math.h>
#include </usr/lib/x86_64-linux-gnu/openmpi/include/mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <strings.h>

#define Cx 0.1f
#define Cy 0.1f

struct Multiplicacion{
	int numero1;
	int numero2;
};
struct Multiplicacion getMultiplicacionBalanceada(int n){
	//Consigue i*j=n más balanceado para armar la matriz
	int raizEntera=(int)sqrt((double)n);
	int divisor=raizEntera;
	//Conseguir el divisor más grande menor a la raíz cuadrada
	while(divisor>0 && n%divisor!=0){
		divisor--;
	}
	//Conseguido el dividor, calcular el dividendo y retornar el objeto Multiplicacion
	int dividendo=n/divisor;

	struct Multiplicacion retornar={
		dividendo,
		divisor
	};
	return retornar;
}

double sampleTime() {
	struct timespec tv;
	clock_gettime(CLOCK_MONOTONIC,&tv);
	return ((double)tv.tv_sec+((double)tv.tv_nsec)/1000000000.0);
}

int main(int argc, char *argv[]) {
	MPI_Init(&argc,&argv);
	register int p, i, j; //Variables para iteración de bucles
	if(argc != 3) {
		printf("Parámetros: Tlado pasos\n");
		exit(1);
	}
	int Tlado = atoi(argv[1]); //N --> Tamaño de la matriz cuadrada
	
	int cantProcesos;
	MPI_Comm_size(MPI_COMM_WORLD,&cantProcesos);
	//Por ahora se hace con una matriz con Tlado divisible por la cantidad de procesos
	if(Tlado%cantProcesos!=0){
		printf("No es divisible lado por cantidad de procesos %d\n",cantProcesos);
		exit(1);
	}

	struct Multiplicacion multiplos=getMultiplicacionBalanceada(cantProcesos);


	int pasos = atoi(argv[2]); //M --> Cantidad de pasos
	if (Tlado <= 0 || pasos < 0) {
		printf("ERROR: valores incorrectos en parámetros\n");
		exit(1);
	}
	//TODO: se considera para matriz cuadrada y lado divisible por lado de grilla
	int TladoProc=Tlado/multiplos.numero1;

	//crear grilla cuadrada
	int dims[]={multiplos.numero1,multiplos.numero2};
	int periods[]={0,0};
	MPI_Comm cart;
	MPI_Cart_create(MPI_COMM_WORLD,2,dims,periods,0,&cart);

	//Definir tipo de dato columna
	
	MPI_Datatype tipo_col;
	MPI_Type_vector(TladoProc,1,TladoProc,MPI_FLOAT,&tipo_col);
	MPI_Type_commit(&tipo_col);

	//CALCULAR POSICION EN LA GRILLA
	int rank;
	MPI_Comm_rank(cart,&rank);
	int coords[2];
	MPI_Cart_coords(cart,rank,2,coords);
	int filaP=coords[0];
	int columnaP=coords[1];
	int columnas = TladoProc;
	int filas = TladoProc;
	int columnasT=Tlado;
	int filasT=Tlado;

	float *fila_arriba = malloc(sizeof(float)*columnas);
	float *fila_abajo = malloc(sizeof(float)*columnas);
	float *columna_izquierda = malloc(sizeof(float)*filas);
	float *columna_derecha = malloc(sizeof(float)*filas);
	
	int rank_arriba;
	int rank_abajo;
	MPI_Cart_shift(cart,0,1,&rank_arriba,&rank_abajo);

	int rank_izq;
	int rank_der;

	MPI_Cart_shift(cart,1,1,&rank_izq,&rank_der);

	int borde_sup_send	;

	//Solo para los procesos con bordes externo, se inicializa en cero
	if(rank_arriba==MPI_PROC_NULL)
		bzero(fila_arriba, sizeof(float)*columnas);
	if(rank_abajo==MPI_PROC_NULL)
		bzero(fila_abajo, sizeof(float)*columnas);
	if(rank_izq==MPI_PROC_NULL)
		bzero(columna_izquierda, sizeof(float)*filas);
	if(rank_der==MPI_PROC_NULL)
		bzero(columna_derecha, sizeof(float)*filas);

	float **matrizActual;
	float **matrizSiguiente;
	float **aux; //Puntero auxiliar para intercambio de punteros

	//Reserva de espacio para la matriz Actual
	matrizActual = (float **)malloc(sizeof(float *) * TladoProc);
	//en matrizActual[0] se apunta el primer elemento de la matri completa
	*matrizActual = (float *)malloc(sizeof(float) * TladoProc * TladoProc);

	//Reserva de espacio para la matriz Siguiente
	matrizSiguiente  = (float **)malloc(sizeof(float *) * TladoProc);
	*matrizSiguiente = (float *)malloc(sizeof(float) * TladoProc * TladoProc);

	if (matrizActual == NULL || matrizSiguiente == NULL){
		printf("ERROR: No se pudo reservar memoria\n");
		exit(2);
	}

	//Se calculan las direcciones de las filas
	for(i = 1; i < TladoProc; i++){
		//Para i=0 ya se hizo antes
		//matrizActual[i] tiene puntero a la fila
		matrizActual[i] = *matrizActual + TladoProc * i;
		matrizSiguiente[i] = *matrizSiguiente + TladoProc * i;
	}

	printf("Inicializando\n");
	//Inicialización de la matriz Actual
	for (i = 0; i < Tlado; i++)
		for (j = 0; j < Tlado; j++)
			if(i>=filaP*TladoProc && i<((filaP+1)*TladoProc)
					&& j>=columnaP*TladoProc && j<((columnaP+1)*TladoProc)
				){
			int i2=i%filas;
			int j2=j%columnas;
			/* printf("rank %d [%d][%d],\n",rank,i,j); */
			matrizActual[i2][j2] = (float)(i+1) * (Tlado + i) * (j+1) * (Tlado + j);
			/* printf("rank %d [%d][%d]: %f,\n",rank,i,j,matrizActual[i2][j2]); */
			}

	printf("Fin inicio\n");
	/* printf("proceso: %d, fila: %d, col: %d\n",rank,coords[0],coords[1]); */
	/* exit(0); */

	//Ejecución de los pasos de simulación
	double time_spent = sampleTime();

	float e_arriba, e_abajo, e_izq, e_der, yo;

	MPI_Request send_arriba=MPI_REQUEST_NULL;
	MPI_Request send_abajo=MPI_REQUEST_NULL;
	MPI_Request send_izq=MPI_REQUEST_NULL;
	MPI_Request send_der=MPI_REQUEST_NULL;


	MPI_Request recv_arriba=MPI_REQUEST_NULL;
	MPI_Request recv_abajo=MPI_REQUEST_NULL;
	MPI_Request recv_izq=MPI_REQUEST_NULL;
	MPI_Request recv_der=MPI_REQUEST_NULL;

	for (p = 0; p < pasos; p++) {

		//INFORMAR Y RECIBIR BORDES
		if(rank_arriba!=MPI_PROC_NULL){
			printf("rank: %d, envía a %d\n",rank,rank_arriba);
			MPI_Isend(matrizActual[0],TladoProc,MPI_FLOAT,rank_arriba,0,cart,&send_arriba);
			MPI_Irecv(fila_arriba,TladoProc,MPI_FLOAT,rank_arriba,MPI_ANY_TAG,cart,&recv_arriba);
		}
		if(rank_abajo!=MPI_PROC_NULL){
			MPI_Isend(matrizActual[TladoProc-1],TladoProc,MPI_FLOAT,rank_abajo,0,cart,&send_abajo);
			MPI_Irecv(fila_abajo,TladoProc,MPI_FLOAT,rank_abajo,MPI_ANY_TAG,cart,&recv_abajo);
		}
		if(rank_izq!=MPI_PROC_NULL){
			MPI_Isend(matrizActual[0],1,tipo_col,rank_izq,0,cart,&send_izq);
			MPI_Irecv(columna_izquierda,TladoProc,MPI_FLOAT,rank_izq,MPI_ANY_TAG,cart,&recv_izq);
		}
		if(rank_der!=MPI_PROC_NULL){
			MPI_Isend(&matrizActual[0][TladoProc-1],1,tipo_col,rank_der,0,cart,&send_der);
			MPI_Irecv(columna_derecha,TladoProc,MPI_FLOAT,rank_der,MPI_ANY_TAG,cart,&recv_der);
		}

		/* printf("TladoProc: %d,filas: %d, columnas: %d\n",TladoProc,filas,columnas); */
		//Se procesa el interior
		for (i = 1; i < filas-1; i++)
			for (j = 1; j < columnas-1; j++) {
				//Comprobar que esté dentro de nuestra submatriz
				yo = matrizActual[i][j];
				e_arriba = matrizActual[i-1][j];
				e_abajo = matrizActual[i+1][j];
				e_izq = matrizActual[i][j-1];
				e_der = matrizActual[i][j+1];
				matrizSiguiente[i][j] = yo+Cx*(e_abajo+e_arriba-2*yo)+Cy*(e_der+e_izq-2*yo);
			}
		//Se procesa fila superior
		i = 0;

		printf("OK\n");
		MPI_Wait(&recv_arriba,MPI_STATUS_IGNORE);

		for (j = 1; j < columnas-1; j++) {
			yo = matrizActual[i][j];
			e_arriba = fila_arriba[j];
			e_abajo = matrizActual[i+1][j];
			e_izq = matrizActual[i][j-1];
			e_der = matrizActual[i][j+1];
			matrizSiguiente[i][j] = yo+Cx*(e_abajo+e_arriba-2*yo)+Cy*(e_der+e_izq-2*yo);

		}

		MPI_Wait(&recv_abajo,MPI_STATUS_IGNORE);

		//Se procesa fila inferior
		i = filas - 1;
		for (j = 1; j < columnas-1; j++) {
			yo = matrizActual[i][j];
			e_arriba = matrizActual[i-1][j];
			e_abajo = fila_abajo[j];
			e_izq = matrizActual[i][j-1];
			e_der = matrizActual[i][j+1];
			matrizSiguiente[i][j] = yo+Cx*(e_abajo+e_arriba-2*yo)+Cy*(e_der+e_izq-2*yo);
		}

		MPI_Wait(&recv_izq,MPI_STATUS_IGNORE);
		//Se procesa columna izquierda
		j = 0;
		for (i = 1; i < filas-1; i++) {
			yo = matrizActual[i][j];
			e_arriba = matrizActual[i-1][j];
			e_abajo = matrizActual[i+1][j];
			e_izq = columna_izquierda[i];
			e_der = matrizActual[i][j+1];
			matrizSiguiente[i][j] = yo+Cx*(e_abajo+e_arriba-2*yo)+Cy*(e_der+e_izq-2*yo);
		}

		MPI_Wait(&recv_der,MPI_STATUS_IGNORE);
		//Se procesa columna derecha
		j = columnas - 1;
		for (i = 1; i < filas-1; i++) {
			yo = matrizActual[i][j];
			e_arriba = matrizActual[i-1][j];
			e_abajo = matrizActual[i+1][j];
			e_izq = matrizActual[i][j-1];;
			e_der = columna_derecha[i];
			matrizSiguiente[i][j] = yo+Cx*(e_abajo+e_arriba-2*yo)+Cy*(e_der+e_izq-2*yo);
		}

		//Se procesa esquina superior izquierda
		i = 0; j = 0;
		yo = matrizActual[i][j];
		e_arriba = fila_arriba[0];
		e_abajo = matrizActual[i+1][j];
		e_izq = columna_izquierda[0];
		e_der = matrizActual[i][j+1];
		matrizSiguiente[i][j] = yo+Cx*(e_abajo+e_arriba-2*yo)+Cy*(e_der+e_izq-2*yo);

		//Se procesa esquina superior derecha
		i = 0; j = columnas-1;
		yo = matrizActual[i][j];
		e_arriba = fila_arriba[columnas-1];
		e_abajo = matrizActual[i+1][j];
		e_izq = matrizActual[i][j-1];
		e_der = columna_derecha[i];
		matrizSiguiente[i][j] = yo+Cx*(e_abajo+e_arriba-2*yo)+Cy*(e_der+e_izq-2*yo);

		//Se procesa esquina inferior izquierda
		i = filas-1; j = 0;
		yo = matrizActual[i][j];
		e_arriba = matrizActual[i-1][j];
		e_abajo = fila_abajo[j];
		e_izq = columna_izquierda[filas-1];
		e_der = matrizActual[i][j+1];
		matrizSiguiente[i][j] = yo+Cx*(e_abajo+e_arriba-2*yo)+Cy*(e_der+e_izq-2*yo);

		//Se procesa esquina inferior derecha
		i = filas-1; j = columnas-1;
		yo = matrizActual[i][j];
		e_arriba = matrizActual[i-1][j];
		e_abajo = fila_abajo[j];
		e_izq = matrizActual[i][j-1];
		e_der = columna_derecha[i];
		matrizSiguiente[i][j] = yo+Cx*(e_abajo+e_arriba-2*yo)+Cy*(e_der+e_izq-2*yo);

		//Se intercambian los punteros para que matrizActual contenga los valores de temperatura recientemente calculados
		aux = matrizActual;
		matrizActual = matrizSiguiente;
		matrizSiguiente = aux;
	} //Fin de pasos

	time_spent = sampleTime() - time_spent;
	printf("Tlado: %d, Pasos: %d\n", Tlado, pasos);
	printf("Tiempo de ejecución: %.5f segundos.\n", time_spent);

	//Se almacena la matriz final en un archivo
	char nombre[30];
	i = filaP;
	j = columnaP;
	sprintf(nombre, "subgrid_%d_%d.out", i, j);
	FILE *f = fopen(nombre, "w");
	if (f == NULL) {
		printf("ERROR: No se pudo abrir el archivo\n");
		exit(1);
	}
	for (i = 0; i <  TladoProc; i++) {
		for (j = 0; j < TladoProc; j++)
			fprintf(f, "%8.3f ", matrizActual[i][j]);
		fprintf(f, "\n");
	}
	fclose(f);

	MPI_Finalize();
	return 0;
}

