#include <stdio.h>
#include <stdlib.h>


struct node {
	struct node 	*left;
	struct node 	*right;
	int 		data;
};

struct node *
get_new_node(int data)
{
	struct node *n = calloc(1, sizeof (struct node));

	n->data = data;
	return n;
}


void
add_node(struct node *root,
	int data)
{
	struct node *t = root;
	struct node *p = NULL;

	while (t) {
		if (t->data == data) {
			return;
		} else if (t->data < data) {
			p = t;
			t = t->right;
		} else {
			p = t;
			t = t->left;
		}
	}

	t = get_new_node(data);
	if (p->data > data) {
		p->left = t;
	} else {
		p->right = t;
	}
	return;
}

void
traverse_preorder(struct node *root)
{
	if (!root) {
		return;
	}
	traverse_preorder(root->left);
	printf("\t%d\t", root->data);
	traverse_preorder(root->right);
	return;
}

int
find_depth(struct node *root)
{
	int depth = 0;

	while (root) {
		root = root->left;
		depth++;
	}
	return depth-1;
}

int
check_depth(struct node *root,
	int depth_left, int cur_depth)
{
	if (!root->left && !root->right) {
		if (cur_depth == depth_left) 
			return 1;
		else 
			return 0;
	} else {
		return ((root->left ? check_depth(root->left, depth_left, cur_depth+1) : 1) &&
		       (root->right ? check_depth(root->right, depth_left, cur_depth+1) : 1));
	}
}

void
main()
{
	struct node 	*root;
	int	 	depth = 0;

	root = get_new_node(10);
	traverse_preorder(root);

	depth = find_depth(root);
	printf("the depth of left-most leaf is %d\n", depth);
	printf("is all leaves at same depth? %d\n", check_depth(root, depth, 0));
	return;
}
