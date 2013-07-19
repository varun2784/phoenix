#include <stdio.h>
#include <stdlib.h>

/*
 * x_1 = 1
 * x_2 = 2
 *
 * xn = xn_1 + xn_2
 */

int sum_even_fibonacci(int limit)
{
	int sum = 0;
	int x = 2, y = 8, z;
	while (x <= limit) {
		printf(" x %d\n", x);
		z = 4 * x + y;
		sum += x;
		y = z;
		x = y;
	}
	return sum;
}

int main(int argc, char **argv)
{
	int sum = sum_even_fibonacci(100);
	return 0;
}
