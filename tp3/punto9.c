#include <stdio.h>
#include <xmmintrin.h>
int main(){
	// float arreglo[4]={12.32,343.2,235.5,123.2};
	float *arreglo;
	posix_memalign((void **)&arreglo,16,sizeof(float)*4);


	printf("Dirección de arreglo: %p\n",arreglo);
	arreglo[0]=(float)12.0;
	arreglo[1]=(float)13.0;
	arreglo[2]=(float)1.0;
	arreglo[3]=(float)2.0;


	//-------------------------------------------------------------

	float *arreglo2;
	posix_memalign((void **)&arreglo2,16,sizeof(float)*4);

	printf("Dirección de arreglo: %p\n",arreglo2);
	arreglo2[0]=(float)1.0;
	arreglo2[1]=(float)1.0;
	arreglo2[2]=(float)1.0;
	arreglo2[3]=(float)5.0;

	//Carga y suma vectorial
	__m128 a=_mm_load_ps(arreglo);
	__m128 b=_mm_load_ps(arreglo2);
	__m128 dest = _mm_add_ps(a,b);

	printf("0: %f\n",dest[0]);
	printf("1: %f\n",dest[1]);
	printf("2: %f\n",dest[2]);
	printf("3: %f\n",dest[3]);

	//Suma escalar
	printf("Resultado final: %f\n",dest[0]+dest[1]+dest[2]+dest[3]);
	// __m128 b= _mm_load_ps(arreglo);

}
