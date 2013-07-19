#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int is_prime(long  m) {
	int isprime = 0;
	long j;
	for (j = 2; j <= sqrt(m); j++) {
		if (m%j == 0) {
			return isprime;
		}
	}
	isprime = 1;
	return isprime;
}

long find_largestprimefactor(long n) {
	long i = sqrt(n);
	while (i >= 2) {
		if (n%i == 0 && is_prime(i)) {
			return i;
		}
		i--;
	}
	return -1;
}
int main (int argc, char **argv)
{
	long m = atol(argv[1]);
	long n = find_largestprimefactor(m);
	printf("largest prime factor is %lu\n", n);
	return 0;
}
