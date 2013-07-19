#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ispalindrom(char *s) {
	int len = strlen(s);
	char *end = s + len -1;
	char *start = s;
	while (start <= end) {
		if (*start != *end) {
		       return 0;
		}
		start++;
		end--;
	}
	return 1;
}
int main() {
	int i,j, k, len;
	int max = 0;
	char str[7];
	for (i=999; i>=1; i--) {
		for (j=i; j>=1; j--) {
			k = i*j;
			sprintf(str,"%d",k);
			if (ispalindrom(str)) {
				if (k >= max) {
					max = k;
				}
			}
		}
	}
	if (max) {
		printf("%d\n", max);
	}
	return 0;
}
