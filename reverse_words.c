#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void
usage(argv)
	char **argv;
{
	printf("Usage: %s <sentence>\n", argv[0]);
	return;
}

void
reverse_words(s)
	char *s;
{
	int len;
	char *start, *tmpstart, *tmpend, *end;
	char ch;
	int blanks = 0;
	int *blankindex =  NULL;
	int index = 0, i =0;
	if (!s) {
		return;
	}
	len = strlen(s);
	start = s;
	end = s+len-1;
	while (start <= end) {
		if (*start == ' ') {
			printf("blank here\n");
			blanks++;
		}
		if (*end == ' ') {
			printf("blank here\n");
			blanks++;
		}
		ch = *start;
		*start = *end;
		*end = ch;
		start++;
		end--;
	}

	/*
	 * revere words
	 * my name is chris
	 * chris is name my
	 */

	start = end = s;
	blankindex = (int *)malloc((blanks+1)*sizeof(int));
	start = s;
	i = 0;
	while (index  < len) {
		start = s+index;
		if (*start == ' ') {
			blankindex[i] = index;
			i++;
		}
		index++;
	}
	blankindex[i]=len;
	start = s;
	printf("%s\n", s);
	for (i=0; i<= blanks;i++) {
		printf("i-%d blankindex %d\n", i, blankindex[i]);
		tmpstart = start;
		tmpend = end = s + blankindex[i]-1;
		while (tmpstart <= tmpend) {
			printf("%c-%c\n", *tmpstart, *tmpend);
			ch = *tmpstart;
			*tmpstart = *tmpend;
			*tmpend = ch;
			tmpstart++;
			tmpend--;
		}
		start = end + 2;
		
	}
	free(blankindex);
}

int main(argc, argv)
	int argc;
	char **argv;
{
	if (argc < 2) {
		usage(argv);
		return 0;
	}

	reverse_words(argv[1]);
	printf("reversed sentence:%s\n", argv[1]);
	return 0;
}
