#include <stdio.h>
#include <stdlib.h>
// #define X 6
// #define Y 6
#define X 20000
#define Y 20000
int main() {
	int i, j;
	int **m = malloc(X * sizeof(int*)); //Una sola fila de punteros
	int *x = malloc(X * Y * sizeof(int)); //grilla
	// printf("Tamaño de **m %d\n",sizeof(m));
	// printf("Tamaño de int*: %d\n",sizeof(int*));
	// printf("Tamaño de int: %d\n",sizeof(int));
	for (i = 0; i < X; i++)
		m[i] = &x[i*Y];//Apunta al primer elemento de cada fila de *x
	for(i = 0; i < X; i++){
		for(j = 0; j < Y; j++){
			m[i][j] = j % 2; // 0 1 0
			// printf("%d ",m[i][j]);
		}
		// printf("\n");
	}
	return 0;
}
