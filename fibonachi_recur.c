#include <stdio.h>
#include <stdlib.h>


int fibo(int *arr, int k) {
	if (*(arr+k-1) != -1) {
		return (*(arr+k-1));
	}

	if (k < 2) {
		*(arr+k-1) = k;
	} else {
		*(arr+k-1)=fibo(arr, k-1)+fibo(arr, k-2);
	}
	return *(arr+k-1);
}
int main (int argc, char **argv) {
	int n = atoi(argv[1]), i;
	int *fb_arr = (int *)malloc(n * sizeof(int));
	for (i = 0; i<n; i++) {
		*(fb_arr+i)=-1;
	}
	printf("fib(%d)=%d\n", n, fibo(fb_arr, n));
	return 0;
}
