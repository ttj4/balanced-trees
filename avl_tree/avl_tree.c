#include <stdio.h>
#include <stdlib.h>

struct node {
	int data;
	struct node *left;
	struct node *right;
	int height;
};


struct node *newNode(int key)
{
	struct node *node = (struct node *)malloc(sizeof(struct node));
	node->data = key;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;
	return node;

}

int max(int a,int b)
{
	return (a > b)?a:b;
}

int findHeight(struct node *node)
{
	if (node == NULL)
		return 0;
	else
		return node->height;
}

struct node *rightRotate(struct node *node)
{
	struct node *p = node->left;
	struct node *t = p->right;
//rotate 
	p->right = node;
	node->left = t;

	node->height = max(findHeight(node->left),findHeight(node->right))+1;
	p->height = max(findHeight(p->left),findHeight(p->right))+1;
//new root
	return p;
}

struct node *leftRotate(struct node *node)
{
	struct node *p = node->right;
	struct node *t = p->left;

	p->left = node;
	node->right = t;

	node->height = max(findHeight(node->left),findHeight(node->right))+1;
	p->height = max(findHeight(node->left),findHeight(node->right))+1;

	return p;
}

int checkHeight(struct node *node)
{
	if (node == NULL)
		return 0;
	return findHeight(node->left)-findHeight(node->right);
}

struct node *insert(struct node *node,int key)
{
	if (node == NULL)
		return newNode(key);
	
	if (key < node->data)
		node->left = insert(node->left,key);
	else
		node->right = insert(node->right,key);
	
	node->height = max(findHeight(node->left),findHeight(node->right))+1;

	int heightFactor = checkHeight(node);
	//left child left subtree
	if (heightFactor > 1 && key < node->left->data)
		return rightRotate(node);
	//right child right subtree
	if (heightFactor < -1 && key > node->right->data)
		return leftRotate(node);
	//left child right subtree
	if (heightFactor > 1 && key > node->left->data) {
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}
	//right child left subtree
	if (heightFactor < -1 && key < node->right->data) {
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	//if unchanged
	return node;
}

struct node *smallestNode(struct node *node)
{
	struct node *p = node;
	while (p->left != NULL)
		p = p->left;
	
	return p;
}

struct node *deleteNode(struct node *root, int key)
{
	//basic bst deletion!

	if (root == NULL)
		return root;

	if (key < root->data)
		root->left = deleteNode(root->left,key);

	else if (key > root->data)
		root->right = deleteNode(root->right,key);

	else {
		//1.no child or one child(either left or right)
		if ((root->left == NULL) || (root->right == NULL)) {
			struct node *temp = root->left ? root->left : root->right;

			//no child
			if (temp == NULL) {
				temp = root;
				root = NULL;
			}
			else // one child
				*root = *temp;//copy data

			free(temp);
		}
		else {
			//2.two children,find smallest node on right subtree and copy contents to root then delete that node
			struct node *temp = smallestNode(root->right);

			root->data = temp->data;
			root->right = deleteNode(root->right,temp->data);
		}
	}

	if (root == NULL)
		return root;
	
	root->height = max(findHeight(root->left),findHeight(root->right))+1;

	int heightFactor = checkHeight(root);

	//left left
	if (heightFactor > 1 && checkHeight(root->left) >= 0)
		return rightRotate(root);
	//left right
	if (heightFactor > 1 && checkHeight(root->left) < 0) {
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}
	//right right
	if (heightFactor < -1 && checkHeight(root->right) <= 0)
		return leftRotate(root);

	//right left
	if (heightFactor < -1 && checkHeight(root->right) > 0) {
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	return root;
}



void inOrder(struct node *p)
{
	if (p != NULL) {
		inOrder(p->left);
		printf("%d ",p->data);
		inOrder(p->right);
	}
}

main()
{
	struct node *root = NULL;
	struct node *node = NULL;
	root = insert(root,18);
	root = insert(root,16);
	root = insert(root,22);
	root = insert(root,20);
	root = insert(root,23);
	root = insert(root,21);
	root = insert(root,27);
	root = insert(root,25);
	root = insert(root,1);
	node = insert(root,11);
	
	printf("========AVL TREE==========\n");

	inOrder(root);
	printf("\n------------------------");
	printf("\nafter deletion\n");
	root = deleteNode(node,25);
	inOrder(root);

	printf("\n=========================\n");
}



