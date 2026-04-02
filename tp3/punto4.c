int pordos (int x) {
	return x * 2;
}
int main() {
	register unsigned int i, res = 0;
	for (i = 0; i < 500000000; i++)
		res += pordos(i);
	return 0;
}
