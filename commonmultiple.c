#include <stdio.h>
#include <stdlib.h>
int find_cm(int a, int b) {
	int i = 1, j = 1;
	int x=a, y=b;
	while (x != y) {
		if (x < y) {
			x = x + a;
		} else {
			y = y + b;
		}
	}
	return x;
}

int main (int argc, char **argv) {
	int s = atoi(argv[1]);
	int t = atoi(argv[2]);
	printf("lcm(%d, %d) = %d\n", s , t, find_cm(s,t));
	return 0;
}
