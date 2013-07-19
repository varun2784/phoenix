#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct node {
	int x;
	struct node *next;
};

int 
add (struct node *n1, struct node *n2, int *level) {
	int l = 0;
	int sum = 0;
	if (!n1 || !n2) {
		exit(EXIT_FAILURE);
	}
	if (n1->next == NULL && n2->next == NULL) {
		*level = 1;
		return (n1->x + n2->x);
	} else {
		sum = add(n1->next, n2->next, &l);
		*level = l + 1;
		return (pow(10, l)*(n1->x + n2->x) + sum);
	}
}

void
insert(struct node **n, int x) {
	struct node *tmp;
	if (*n == NULL) {
		*n = malloc(sizeof (struct node));
		(*n)->next = NULL;
		(*n)->x = x;
	} else {
		tmp = (*n)->next;
		(*n)->next = malloc(sizeof (struct node));
		(*n)->next->x = x;
		(*n)->next->next = tmp;
	}
	return;
}

int
main (int argc, char **argv) {
	struct node *n1 = NULL, *n2 = NULL;
	int sum = 0, l;
	insert(&n1, 9);
	insert(&n1, 9);
	printf("%p\n", n1);
	insert(&n2, 9);
	insert(&n1,9);
	insert(&n1,9);
	printf("%p\n", n1);
	insert(&n2,9);
	insert(&n2,9);
	insert(&n2,9);
	sum = add(n1,n2, &l);
	printf("sum is %d\n", sum);
	return 0;
}
