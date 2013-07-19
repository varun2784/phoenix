#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
	int a = 0x123456;
	int *ptra = &a;
	if (*ptra == 0x12) {
		printf("big endian\n");
	} else {
		printf("little endian %x\n", *ptra);
	}
	return 0;
}
