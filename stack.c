#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * stack[100];
int top = -1;

char *
pop(char **stack) {
	char *item = stack[top];
	top--;
	return item;
}

void
push(char **stack, char *str) {
	printf("top %d\n", top);
	top++;
	stack[top] =  str;
	return;
}

int main(int argc, char **argv) {
	char limit = ' ';
	char *sentence = argv[1];
	char *token;
	token = strtok(sentence, &limit);
	if (!token) {
		return;
	}
	printf("%s\n", token);
	push(stack, token);
	while (token = strtok(NULL, &limit)) {
		printf("%s\n", token);
		push(stack, token);
	}
	while (top >= 0) {
		token = pop(stack);
		printf("%s ", token);
	}
	return 0;	
}

