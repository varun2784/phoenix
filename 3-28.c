#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/*
 * array = [4.7.9.2.5]
 */

int multiply(int arr[], int start, int end) {
	int mid;
	printf("start %d end %d\n", start, end);
	if (start > end) {
		return 1;
	}
	if (start == end) {
		return arr[start];
	}
	mid = (start+end)/2;
	return (multiply(arr, start, mid)*multiply(arr, mid+1, end));
}

int compute_mi(int arr[], int i, int n) {
	if ((i >= n) || (i < 0)) {
		return 0;
	}

	return (multiply(arr, 0, i-1)*multiply(arr, i+1, n-1)); 
}

int main(int argc, char **argv) {
	int array[]={4, 7, 9, 2, 5,10};
	int mi[]={1, 1, 1, 1, 1, 1};
	int i;
	for (i=0; i<=5; i++) {
		mi[i]=compute_mi(array, i, 6);
		printf("i %d mi %d\n", i, mi[i]);
	}
	return 0;
}
