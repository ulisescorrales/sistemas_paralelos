
#include <smmintrin.h>
#include <stdlib.h>
#include <stdio.h>
#include <xmmintrin.h>

int main() {
	int i, elementos = 100000000;
	int *v;
	posix_memalign((void **)&v,16,elementos*sizeof(int));

	int *puntero=v;
	
	/* float *arreglo; */


	__m128i v1;
	__m128i suma;
	__m128i resta;
	__m128i cinco_pos=_mm_set1_epi32(5);
	__m128i cincuenta_millones=_mm_set1_epi32(50000000);
	__m128i mask;
	__m128i resultado;
	//no vectorizar este for
	for (i = 0; i < elementos; i++)
		v[i] = i;

	for (i = 0; i < elementos; i=i+4){
		v1=_mm_load_si128((__m128i *)puntero);
		mask=_mm_cmpgt_epi32(v1,cincuenta_millones);

		suma=_mm_add_epi32(v1,cinco_pos);
		resta=_mm_sub_epi32(v1,cinco_pos);

		/* resultado=_mm_blendv_epi8(suma,resta,mask); */

		__m128i resultado = _mm_or_si128(
            _mm_and_si128(mask, resta),      // Valores que cumplen la condición
            _mm_andnot_si128(mask, suma)     // Valores que NO la cumplen
        );
		//Guardar resultado
		_mm_store_si128((__m128i*)puntero,resultado);

		puntero=puntero+4;
	}

	printf("elemento 0: %d\n",v[0]);
	printf("elemento 50000000: %d\n",v[50000000]);
	printf("elemento 50000001: %d\n",v[50000001]);
	printf("elemento 999999999: %d\n",v[99999999]);

	return 0;
}

