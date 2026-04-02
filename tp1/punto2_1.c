#include <stdio.h>
int main(){
	float arreglo[5];
	float *pos5=&arreglo[4];
	*pos5=10.4;

	printf("%f\n",arreglo[4]);
	return 0;
}
