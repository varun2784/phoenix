#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void process(unsigned long long *arr, int n) {
	unsigned long long   cur, x, y;
	int i;
	for (i = 1; i <= n; i++) {
		cur = *(arr + i -1);
		y = cur % (unsigned long long)pow(n,i);
		x = pow(n,y);
		*(arr + y - 1) += x;
	}
	for (i = 1; i <= n; i++) {
		cur = *(arr + i - 1);
		*(arr + i - 1) = cur/pow(n,i);
	}
	for (i = 1; i <=n ; i++) {
		printf("%d repeated %llu times\n", i, *(arr+i-1));
	}
}

int main (int argc, char **argv) {
	unsigned long long array[10];
	array[0]=4;
	array[1]=3;
	array[2]=5;
	array[3]=1;
	array[4]=2;
	array[5]=9;
	array[6]=4;
	array[7]=2;
	array[8]=7;
	array[9]=10;
	process(array, 10);
	return 0;
}
