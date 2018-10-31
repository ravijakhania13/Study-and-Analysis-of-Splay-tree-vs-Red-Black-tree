#include <iostream>
#include <stdlib.h>
using namespace std;

struct SplayNode
{
    int value;
    int height;
    int left_num_of_element;
    int right_num_of_element;
    struct SplayNode *left;
    struct SplayNode *right;
};
struct SplayNode *Head;
struct SplayNode* make_new_node(int val)
{
	struct SplayNode *temp=(struct SplayNode*) malloc(sizeof(struct SplayNode));
	temp->value=val;
	temp->height=1;
	temp->left_num_of_element=0;
    temp->right_num_of_element=0;
	temp->left=NULL;
	temp->right=NULL;
	return temp;
}
int maximum(int a,int b)
{
	if(a<b)
		return b;
	else
		return a;
}
int find_b_factor(struct SplayNode* currentnode)
{
	int B_factor;
	if(currentnode->left==NULL && currentnode->right==NULL)
		return 0;
	else if(currentnode->left==NULL)
		B_factor=0-currentnode->right->height;
	else if(currentnode->right==NULL)
		B_factor=currentnode->left->height-0;
	else
		B_factor=currentnode->left->height-currentnode->right->height;
	return B_factor;
}
void height_update(struct SplayNode* currentnode)
{
	if(currentnode->left==NULL && currentnode->right==NULL)
		currentnode->height=1;
	else if(currentnode->left==NULL)
		currentnode->height=currentnode->right->height+1;
	else if(currentnode->right==NULL)
		currentnode->height=currentnode->left->height+1;
	else
		currentnode->height=maximum(currentnode->left->height,currentnode->right->height)+1;
}
void num_of_element_update(struct SplayNode* currentnode)
{
	if(currentnode->left==NULL)
		currentnode->left_num_of_element=0;
	else
		currentnode->left_num_of_element=currentnode->left->left_num_of_element+currentnode->left->right_num_of_element+1;

	if(currentnode->right==NULL)
		currentnode->right_num_of_element=0;
	else
		currentnode->right_num_of_element=currentnode->right->left_num_of_element+currentnode->right->right_num_of_element+1;
}

struct SplayNode* zig(struct SplayNode *currentnode)
{
	struct SplayNode *temp=currentnode->left;
	currentnode->left=currentnode->left->right;
	temp->right=currentnode;
	return temp;
}
struct SplayNode* zag(struct SplayNode *currentnode)
{
	struct SplayNode *temp=currentnode->right;
	currentnode->right=currentnode->right->left;
	temp->left=currentnode;
	return temp;	
}
struct SplayNode* zigzig(struct SplayNode *currentnode)
{
	struct SplayNode *temp=currentnode->left;
	currentnode->left=currentnode->left->right;
	temp->right=currentnode;
	currentnode=temp;
	currentnode=zig(currentnode);
	return currentnode;
}
struct SplayNode* zagzig(struct SplayNode *currentnode)
{
	struct SplayNode *temp=currentnode->left->right;
	currentnode->left->right=currentnode->left->right->left;
	temp->left=currentnode->left;
	currentnode->left=temp;
	currentnode=zig(currentnode);
	return currentnode;
}
struct SplayNode* zagzag(struct SplayNode *currentnode)
{
	struct SplayNode *temp=currentnode->right;
	currentnode->right=currentnode->right->left;
	temp->left=currentnode;
	currentnode=temp;
	currentnode=zag(currentnode);
	return currentnode;
}
struct SplayNode* zigzag(struct SplayNode *currentnode)
{
	struct SplayNode *temp=currentnode->right->left;
	currentnode->right->left=currentnode->right->left->right;
	temp->right=currentnode->right;
	currentnode->right=temp;
	currentnode=zag(currentnode);
	return currentnode;
}

struct SplayNode* search(struct SplayNode* currentnode,int val)
{
	if(currentnode == NULL || currentnode -> value == val)
		return currentnode;

	if(currentnode -> value > val)
	{
		if(currentnode -> left == NULL)
			return currentnode;

		if(currentnode -> left ->value > val)
		{
			currentnode -> left -> left = search(currentnode -> left ->  left,val);

			if(currentnode -> left -> left != NULL)
				currentnode = zigzig(currentnode);
			else
				currentnode = zig(currentnode);
		}
		else if(currentnode -> left ->value < val)
		{
			currentnode -> left -> right = search(currentnode -> left ->  right,val);

			if(currentnode -> left -> right != NULL)
				currentnode = zagzig(currentnode);
			else
				currentnode = zig(currentnode);
		}
		else
			currentnode=zig(currentnode);
	}
	else
	{
		if(currentnode -> right == NULL)
			return currentnode;
		if(currentnode -> right -> value < val)
		{
			currentnode -> right -> right = search(currentnode -> right -> right,val);

			if(currentnode -> right -> right != NULL)
				currentnode = zagzag(currentnode);
			else
				currentnode = zag(currentnode);
		}
		else if(currentnode -> right -> value > val)
		{
			currentnode -> right -> left = search(currentnode -> right -> left,val);
			if(currentnode -> right -> left != NULL)
				currentnode = zigzag(currentnode);
			else
				currentnode = zag(currentnode);
		}
		else
			currentnode=zag(currentnode);
	}
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
struct SplayNode* insert(struct SplayNode* currentnode,int val)
{
	struct SplayNode *temp = make_new_node(val);
	if(currentnode == NULL)
		return temp;

	currentnode = search(currentnode,val);
	preorder(currentnode);
	cout<<endl;
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
struct SplayNode* find_predasessor(struct SplayNode* currentnode)
{
	while(currentnode->right!=NULL)
		currentnode=currentnode->right;
	return currentnode;
}

// struct SplayNode* delete_node(struct SplayNode* currentnode,int val)
// {

// 	if(currentnode->value==val)
// 	{
// 		if(currentnode->left==NULL && currentnode->right==NULL)
// 		{
// 			struct SplayNode *temp=currentnode;
// 			free(temp);
// 			return NULL;
// 		}
// 		else if(currentnode->right==NULL)
// 		{
// 			struct SplayNode *temp=currentnode->left;
// 			*(currentnode)=*(currentnode->left);
// 			free(temp);
// 		}
// 		else if(currentnode->left==NULL)
// 		{
// 			struct SplayNode *temp=currentnode->right;
// 			*(currentnode)=*(currentnode->right);
// 			free(temp);
// 		}
// 		else
// 		{
// 			struct SplayNode *temp=find_predasessor(currentnode->left);
// 			currentnode->value=temp->value;
// 			currentnode->left=delete_node(currentnode->left,temp->value);
// 		}
// 	}
// 	else if(val<currentnode->value)
// 		currentnode->left=delete_node(currentnode->left,val);
// 	else
// 		currentnode->right=delete_node(currentnode->right,val);

// 	height_update(currentnode);
// 	num_of_element_update(currentnode);
// 	return balancing(currentnode);

// }


int main()
{

	// struct SplayNode *root = make_new_node(100); 
 //    root->left = make_new_node(50); 
 //    root->right = make_new_node(200); 
 //    root->left->left = make_new_node(40); 
 //    root->left->left->left = make_new_node(30); 
 //    root->left->left->left->left = make_new_node(20); 
  
 //    root = search(root, 20); 
	struct SplayNode *root;
	root= insert(root,30);
	preorder(root);
	cout<<endl;
	root= insert(root,25);
	preorder(root);
	cout<<endl;
	root= insert(root,60);
	preorder(root);
	cout<<endl;
	root= insert(root,70);
	preorder(root);
	cout<<endl;
	root= search(root,27);
    preorder(root);
    cout<<endl;
	return 0;
}