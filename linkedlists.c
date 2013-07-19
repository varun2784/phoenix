#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

struct node {
	int val;
	struct node *next;
};

void
insert(struct node **np, int x) {
	struct node *n = *np;
	if (!(n)) {
		n = malloc(sizeof (struct node));
		if (!n) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		n->next = NULL;
		n->val = x;
		*np = n;
	} else {
		while (n->next) {
			n = n->next;
		}
		n->next = malloc(sizeof (struct node));
		if (!n->next) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		n->next->next = NULL;
		n->next->val = x;
	}
	return;
}

struct node * 
find(struct node **np, int x) {
	struct node *n = *np;
	while (n) {
		if (n->val == x) {
			return n;
		}
		n =  n->next;
	}
	return NULL;
}

struct node *
find_pre(struct node **np, int x, int *foundp) {
	struct node *n = *np, *p = NULL;
	while (n) {
		if (n->val == x) {
			*foundp = 1; 
			return p;
		}
		p = n;
		n = n->next;
	}
	return NULL;
}

void delete(struct node **np, int x) {
	int found = 0;
	struct node *m, *p;
	m = find_pre(np, x, &found);
	if (found) {
		if (m) {
			p = m->next;
			m->next = p->next;
			free(p);
		} else {
			p = *np;
			*np = p->next;
			free(p);
		}
	}
}

void
print(struct node *n) {
	int i = 0;
	while (n) {
		i++;
		printf("i %d val %d\n", i, n->val);
		n = n->next;
	}
}

int main () {
	int i;
	struct node *head = NULL;
	for (i = 0; i < 100; i++) {
		insert(&head, i);
	}
	print(head);
	delete(&head, 0);
	print(head);
	delete(&head, 4);
	for (i = 10; i < 100; i++) {
		delete(&head, i);
	}
	print(head);
	return;
}
