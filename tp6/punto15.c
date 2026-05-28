#define N 2000000000
void vector_add(int *A, int *B, int *C, int n) {
   int i;
   for (i = 0; i < n; i++)
      C[i] = A[i] + B[i];
}

void rec_vector_add(int *A, int *B, int *C, int n) {
// si es el caso base, llamar a vector_add
// sino, hacer la suma recursiva sobre la primera mitad de los arreglos, y luego la suma recursiva sobre la segunda mitad de los arreglos.
	if(n==1){
		
	}
}

int main() {
   rec_vector_add(a, b, c, N);
}

