#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define X 50000
#define Y 10000

int main() {
	int i, j;
	int *m= malloc(X*Y * sizeof(int*));
	int cant=X*Y;
	for(int i=0; i < Y; i++){
		for(int j=0;j < X; j++){
			/* printf("%d\n",i*X+j); */
			m[i*X+j]=i;
		}
	}
	return 0;
}

