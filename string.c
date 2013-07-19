#include <stdio.h>
#include <stdlib.h>

void 
eliminate (char *s)
{
	int move = 0;
	while (*s != '\0') {
		if (move) {
			if ((*s != 'b') && !(*s == 'a' && *(s+1) == 'c')) {
				*(s-move) = *s;
			}
		}
		if (*s == 'b') {
			move++;
		}
		if (*s == 'a' && *(s+1) == 'c') {
			s++;
			move=move+2;
		}
		s++;
	}
	*(s-move)='\0';
	return;
}

int
main (int argc, char **argv)
{
	eliminate(argv[1]);
	printf("%s", argv[1]);
	return 0;
}
