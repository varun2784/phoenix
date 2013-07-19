#include <stdlib.h>
#include <stdio.h>
struct node {
	int 	key;
	struct node * left;
	struct node * right;
};

void
insert(struct node **nodepp, int key) {
	if (!(*nodepp)) {
		*nodepp = malloc(sizeof (struct node));
		(*nodepp)->key = key;
		(*nodepp)->left = NULL;
		(*nodepp)->right = NULL;
	} else {
		if (key == (*nodepp)->key) {
			perror("key exist");
			exit(EXIT_FAILURE);
		}
		if (key < (*nodepp)->key) {
			insert(&((*nodepp)->left), key);
		} else if (key > (*nodepp)->key) {
			insert(&((*nodepp)->right), key);
		}
	}
	return;
}

struct node * 
search(struct node **nodepp, int key) {
	if (*nodepp == NULL) {
		return NULL;
	}
	if ((*nodepp)->key == key) {
		return *nodepp;
	} else if ((*nodepp)->key < key) {
		return search(&((*nodepp)->right), key);
	} else {
		return search(&((*nodepp)->left), key);
	}

}

void print_tree(struct node **nodepp) {
	if (!*nodepp) {
		return;
	}
	print_tree(&(*nodepp)->left);
	printf("%d\t", (*nodepp)->key);
	print_tree(&(*nodepp)->right);
	return;
}

int main (int argc, char **argv) {
	struct node *head = NULL;
	struct node *t = NULL;
	insert(&head, 2);
	printf("%p\n", head);
	insert(&head, 1);
	printf("%p\n", head);
	insert(&head, 4);
	printf("%p\n", head);
	print_tree(&head);
	t = search(&head, 4);
	printf("%p\n", t);
	return 0;
}
