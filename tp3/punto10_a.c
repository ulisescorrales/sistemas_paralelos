#include <emmintrin.h>
#include <emmintrin.h>
#include <stdlib.h>
#include <stdio.h>
#include <smmintrin.h>
int main() {
	int i, elementos = 1000005;
	int *v;
	int ok=posix_memalign((void **)&v,16,elementos*sizeof(int));
	if(ok!=0){
		printf("No se ha podido reservar memoria\n");
		exit(1);
	}

	//1- alinear el malloc inicial
	//2- Definir la variable que recorre el arreglo 
	__m128i recorrer;
	//3- Definir las constante
	__m128i dos=_mm_set1_epi32(2);
	__m128i menos_dos=_mm_set1_epi32(-2);

	//4 - Definir temporales
	__m128i por_dos;
	__m128i por_dos_por_menos_dos;
	__m128i diez=_mm_set1_epi32(10);
	__m128i lado_derecho;
	__m128i lado_izquierdo;
	//5- Definir la máscara
	__m128i mascara;
	__m128i si;
	__m128i no;
	__m128i resultado;
	// no vectorizar este for
	for (i = 0; i < elementos; i++){
		v[i] = rand() % 10000;
	}
	printf("v[0]=%11d, ", v[0]);
	printf("v[11]=%11d, ", v[11]);
	printf("v[elementos-1]=%11d\n", v[elementos-1]);

	int *puntero=v;
	__m128i sub_dos;
	for (i = 0; i < elementos; i=i+4) {
		recorrer=_mm_load_si128((__m128i*)puntero);
		lado_derecho=_mm_set1_epi32(i);
		lado_derecho=_mm_mullo_epi32(lado_derecho,diez);
		
		lado_izquierdo=_mm_mullo_epi32(dos,recorrer);
		mascara=_mm_cmplt_epi32(lado_izquierdo,lado_derecho);

		/* v[i] = v[i] * 2; */
		por_dos=_mm_mullo_epi32(recorrer,dos);
		si=_mm_and_si128(por_dos,mascara);

		/* v[i] = (v[i] - 2) * -2; */
		sub_dos=_mm_sub_epi32(recorrer,dos);
		por_dos_por_menos_dos=_mm_mullo_epi32(sub_dos,menos_dos);
		no=_mm_andnot_si128(mascara,por_dos_por_menos_dos);

		resultado=_mm_or_si128(si,no);
		
		/* printf("%d\n",(int)resultado[0]); */
		/* exit(0); */
		_mm_store_si128((__m128i*)puntero,resultado);
		puntero=puntero+4;
	}
	printf("v[0]=%11d, ", v[0]);
	printf("v[11]=%11d, ", v[11]);
	printf("v[elementos-1]=%11d\n", v[elementos-1]);
	return 0;
}
