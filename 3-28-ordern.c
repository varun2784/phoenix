#include <stdio.h>
#include<stdlib.h>

int main (int argc, char **argv) {
	int array[] = {2, 5, 3, 4};
	int fwdprod[]={1, 1, 1, 1};
	int backprod[]={1, 1, 1, 1};
	int mi[]={1, 1, 1, 1};
	int i, n=4;
	for (i=1; i<=n-1; i++) {
		fwdprod[i]=fwdprod[i-1]*array[i-1];
	}
	for (i=n-2; i>=0; i--) {
		backprod[i] = backprod[i+1]*array[i+1];
	}
	for (i=0;i<=n-1;i++) {
		mi[i]=fwdprod[i]*backprod[i];
		printf("i %d, mi %d\n", i, mi[i]);
	}
	return 0;
}
