#include <stdio.h>
#include <stdlib.h>

int binarysearch(int num, int arr[], int start, int end) {
	int mid = (start + end)/2;
	printf("start %d mid %d end %d\n", start, mid, end);
	if (start <= end) {
		if (num > arr[mid]) {
			return (binarysearch(num, arr, mid+1, end));
		} else if (num < arr[mid]) {
			return (binarysearch(num, arr, start, mid-1));
		} else 
			return mid+1;
	} else {
		return -1;
	}
}

void usage(char *name) {
	printf("%s number len arr[0]...arr[len-1]\n", name);
	return;
}

int main (int argc, char **argv) {
	if (argc < 3) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	int tofind = atoi(argv[1]);
	int len = atoi(argv[2]);
	int i = 0, *arr = NULL, found = -1;
	if (argc != (3 + len)) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	arr = (int *)malloc(len * sizeof (int));
	for (i = 0; i < len; i++) {
		*(arr + i) = atoi(argv[3 + i]);
	}
	found = binarysearch(tofind, arr, 0, len - 1);
	printf("found %d at %d\n", tofind, found);
	free(arr);
	return 0;
}
