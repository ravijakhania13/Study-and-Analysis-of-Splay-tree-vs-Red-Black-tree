#include <iostream>
#include <stdlib.h>
using namespace std;

struct SplayNode
{
    int value;
    struct SplayNode *left;
    struct SplayNode *right;
};

struct SplayNode *Head;

struct SplayNode* make_new_node(int val)
{
	struct SplayNode *temp = (struct SplayNode*) malloc(sizeof(struct SplayNode));
	temp -> value = val;
	temp -> left = NULL;
	temp -> right = NULL;
	return temp;
}

struct SplayNode* zig(struct SplayNode *currentnode)
{
	struct SplayNode *temp = currentnode -> left;
	currentnode -> left = currentnode -> left -> right;
	temp -> right = currentnode;
	return temp;
}

struct SplayNode* zag(struct SplayNode *currentnode)
{
	struct SplayNode *temp = currentnode -> right;
	currentnode -> right = currentnode -> right -> left;
	temp -> left = currentnode;
	return temp;	
}

struct SplayNode* zigzig(struct SplayNode *currentnode)
{
	struct SplayNode *temp = currentnode -> left;
	currentnode -> left = currentnode -> left -> right;
	temp -> right = currentnode;
	currentnode = temp;
	currentnode = zig(currentnode);
	return currentnode;
}

struct SplayNode* zagzig(struct SplayNode *currentnode)
{
	struct SplayNode *temp = currentnode -> left -> right;
	currentnode -> left -> right = currentnode -> left -> right -> left;
	temp -> left = currentnode -> left;
	currentnode -> left = temp;
	currentnode = zig (currentnode);
	return currentnode;
}

struct SplayNode* zagzag(struct SplayNode *currentnode)
{
	struct SplayNode *temp = currentnode -> right;
	currentnode -> right = currentnode -> right -> left;
	temp -> left = currentnode;
	currentnode = temp;
	currentnode = zag(currentnode);
	return currentnode;
}

struct SplayNode* zigzag(struct SplayNode *currentnode)
{
	struct SplayNode *temp = currentnode -> right -> left;
	currentnode -> right -> left = currentnode -> right -> left -> right;
	temp -> right = currentnode -> right;
	currentnode -> right = temp;
	currentnode = zag (currentnode);
	return currentnode;
}

struct SplayNode* search_node(struct SplayNode* currentnode,int val)
{
	if(currentnode == NULL || currentnode -> value == val)
		return currentnode;

	if(currentnode -> value > val)
	{
		if(currentnode -> left == NULL)
			return currentnode;

		if(currentnode -> left ->value > val)
		{
			currentnode -> left -> left = search_node (currentnode -> left ->  left, val);

			if(currentnode -> left -> left != NULL)
				currentnode = zigzig (currentnode);
			else
				currentnode = zig (currentnode);
		}
		else if(currentnode -> left ->value < val)
		{
			currentnode -> left -> right = search_node (currentnode -> left ->  right, val);

			if(currentnode -> left -> right != NULL)
				currentnode = zagzig (currentnode);
			else
				currentnode = zig (currentnode);
		}
		else
			currentnode = zig (currentnode);
	}
	else
	{
		if(currentnode -> right == NULL)
			return currentnode;
		if(currentnode -> right -> value < val)
		{
			currentnode -> right -> right = search_node (currentnode -> right -> right, val);

			if(currentnode -> right -> right != NULL)
				currentnode = zagzag (currentnode);
			else
				currentnode = zag (currentnode);
		}
		else if(currentnode -> right -> value > val)
		{
			currentnode -> right -> left = search_node (currentnode -> right -> left, val);
			if(currentnode -> right -> left != NULL)
				currentnode = zigzag (currentnode);
			else
				currentnode = zag (currentnode);
		}
		else
			currentnode = zag (currentnode);
	}
	return currentnode;
}


struct SplayNode* insert_node(struct SplayNode* currentnode,int val)
{
	struct SplayNode *temp = make_new_node(val);

	if(currentnode == NULL)
		return temp;

	currentnode = search_node(currentnode, val);

	if(currentnode -> value == val)
		return currentnode;
	
	if(currentnode -> value > val)
	{
		temp -> right = currentnode;
		temp -> left = currentnode -> left;
		currentnode -> left = NULL;
	}

	else
	{
		temp -> left = currentnode;
		temp -> right = currentnode -> right;
		currentnode -> right = NULL;
	}

	return temp;
}

struct SplayNode* delete_node(struct SplayNode* currentnode,int val)
{
	struct SplayNode* temp;

	if(currentnode == NULL)
		return currentnode;

	search_node (currentnode, val);

	if(currentnode -> value != val)
		return currentnode;

	if(currentnode -> left != NULL)
	{
		temp = currentnode;
		currentnode = search_node (currentnode -> left, val);
		currentnode -> right = temp -> right;
	}
	else
	{	
		temp = currentnode;
		currentnode = currentnode -> right;
	}
	free (temp);
	return currentnode;
}

void preorder(struct SplayNode *root) 
{ 
    if (root != NULL) 
    { 
        printf("%d ", root->value); 
        preorder(root->left); 
        preorder(root->right); 
    } 
}

int main()
{

	// struct SplayNode *root = make_new_node(100); 
 //    root->left = make_new_node(50); 
 //    root->right = make_new_node(200); 
 //    root->left->left = make_new_node(40); 
 //    root->left->left->left = make_new_node(30); 
 //    root->left->left->left->left = make_new_node(20); 
  
 //    root = search_node(root, 20); 
	struct SplayNode *root;
	root= insert_node (root,30);
	preorder(root);
	cout<<endl;
	root= insert_node (root,25);
	preorder(root);
	cout<<endl;
	root= insert_node (root,60);
	preorder(root);
	cout<<endl;
	root= insert_node (root,70);
	preorder(root);
	cout<<endl;
	root= insert_node (root,27);
    preorder(root);
    cout<<endl;
	return 0;
}