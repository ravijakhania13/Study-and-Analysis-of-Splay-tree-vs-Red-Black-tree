#include <iostream>
using namespace std;

struct SplayNode
{
    int value;
    struct SplayNode *left;
    struct SplayNode *right;

    SplayNode(int v): value(v), left(NULL), right(NULL) {}
};

class SplayTree
{
    SplayNode *root;

    SplayNode* searchUtil(SplayNode*, int);
    void preorderUtil(SplayNode*);
    void inorderUtil(SplayNode*);
    void clearUtil(SplayNode*);

    SplayNode* zig   (SplayNode*);
    SplayNode* zag   (SplayNode*);
    SplayNode* zigzig(SplayNode*);
    SplayNode* zagzig(SplayNode*);
    SplayNode* zagzag(SplayNode*);
    SplayNode* zigzag(SplayNode*);

public:
    SplayTree(): root(NULL) {}

    void insert(int);
    bool search(int);
    void remove(int);
    void preorder() { preorderUtil(root); cout << endl; }
    void inorder() { inorderUtil(root); cout << endl; }

    void clear() { clearUtil(root); root = NULL; }

    ~SplayTree()
    {
        clear();
    }
};


void SplayTree::clearUtil(SplayNode* node)
{
    if(!node) return;

    clearUtil(node->left);
    clearUtil(node->right);
    delete node;
}


// Right Rotate
SplayNode* SplayTree::zig(SplayNode *node)
{
    SplayNode* l = node->left;
    node->left = l->right;
    l->right = node;
    return l;
}

// Left Rotate
SplayNode* SplayTree::zag(SplayNode *node)
{
    SplayNode *r = node->right;
    node->right = r->left;
    r->left = node;
    return r;
}

// Right-Right Rotate
SplayNode* SplayTree::zigzig(SplayNode *node)
{
    node = zig(node);
    return zig(node);
}

// Left-Left Rotate
SplayNode* SplayTree::zagzag(SplayNode *node)
{
    node = zag(node);
    return zag(node);
}

SplayNode* SplayTree::zagzig(SplayNode *node)
{
    node->left = zag(node->left);
    return zig(node);
}

SplayNode* SplayTree::zigzag(SplayNode *node)
{
    node->right = zig(node->right);
    return zag(node);
}

SplayNode* SplayTree::searchUtil(SplayNode* node,int val)
{
    // Base cases: node is NULL or val is present at node
    if(!node || node->value == val)
        return node;

    // val lies in left subtree
    if(node->value > val)
    {
        // Key is not in tree
        if(!node->left) return node;

        if(node->left->value == val)
            return zig(node);

        // Zig-Zig (Left Left)
        if(node->left->value > val)
        {
            // recursively bring val as root of left-left
            node->left->left = searchUtil (node->left->left, val);
            return (node->left->left) ? zigzig(node) : zig(node);
        }

        // Zag-Zig (Left Right)
        // recursively bring val as root of left-right
        node->left->right = searchUtil (node->left->right, val);

        return (node->left->right) ? zagzig(node) : zig(node);
    }
    else    // val lies in right subtree
    {
        // val is not in tree
        if(!node->right) return node;

        if (node->right->value == val)
            return zag (node);

        // Zag-Zag (Right Right)
        if(node->right->value < val)
        {
            // recursively bring val as root of right-right
            node->right->right = searchUtil (node->right->right, val);
            return (node->right->right) ? zagzag(node) : zag(node);
        }

        // Zig-Zag (Right Left)
        // recursively bring val as root of right-left
        node->right->left = searchUtil (node->right->left, val);

        return (node->right->left) ? zigzag(node) : zag(node);
    }
}


bool SplayTree::search(int val)
{
    root = searchUtil(root, val);
    if (root->value != val)
    {
        cout << val << " not found in the SplayTree\n";
        return false;
    }
    cout << val << " is present in the SplayTree\n";
    return true;
}

void SplayTree::insert(int val)
{
    SplayNode* new_node = NULL;

    if(!root)
    {
        root = new SplayNode(val);
        return;
    }

    root = searchUtil(root, val);

    if(root->value == val)
        return;

    new_node = new SplayNode(val);
    if(root->value > val)
    {
        new_node->right = root;
        new_node->left = root->left;
        root->left = NULL;
    }
    else
    {
        new_node->left = root;
        new_node->right = root->right;
        root->right = NULL;
    }
    root = new_node;
}


void SplayTree::remove(int val)
{
    SplayNode* rem_node = NULL;

    if(!root)
        return;

    root = searchUtil (root, val);

    if(root->value != val)
        return;

    rem_node = root;
    if(!root->left)
    {
        root = root->right;
    }
    else
    {
        root = searchUtil (root->left, val);
        root->right = rem_node->right;
    }
    delete rem_node;
    rem_node = NULL;
}


void SplayTree::inorderUtil(SplayNode* node)
{
    if(node)
    {
        inorderUtil(node->left);
        cout << node->value << " ";
        inorderUtil(node->right);
    }
}


void SplayTree::preorderUtil(SplayNode* node)
{
    if(node)
    {
        cout << node->value << " ";
        preorderUtil(node->left);
        preorderUtil(node->right);
    }
}


int main()
{
    SplayTree splay_tree;

    splay_tree.insert(30);
    splay_tree.inorder();

    splay_tree.insert(25);
    splay_tree.inorder();

    splay_tree.insert(60);
    splay_tree.inorder();

    splay_tree.insert(70);
    splay_tree.inorder();

    splay_tree.insert(27);
    splay_tree.inorder();

    splay_tree.search(25);
    splay_tree.search(29);
    splay_tree.search(70);
    splay_tree.search(77);

    splay_tree.remove(70);
    splay_tree.search(70);
    return 0;
}
