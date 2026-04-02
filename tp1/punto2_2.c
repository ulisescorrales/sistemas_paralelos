#include <stdio.h>
int main(){
	int arreglo[3][4];
	int *p;

	for(int i=0;i<3;i++){
		for(int j=0;j<4;j++){
			p=&arreglo[i][j];
			printf("%p - ",p);
		}
		printf("\n");
	}
	return 0;
}

