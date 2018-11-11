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

    SplayNode*                    splay(SplayNode*, int);
    SplayNode*                    join (SplayNode*, SplayNode*);
    pair<SplayNode*, SplayNode*>  split(SplayNode*, int);
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

    bool isEmpty() { return root == NULL; }
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

SplayNode* SplayTree::splay(SplayNode* node,int val)
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
            node->left->left = splay (node->left->left, val);
            return (node->left->left) ? zigzig(node) : zig(node);
        }

        // Zag-Zig (Left Right)
        // recursively bring val as root of left-right
        node->left->right = splay (node->left->right, val);

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
            node->right->right = splay (node->right->right, val);
            return (node->right->right) ? zagzag(node) : zag(node);
        }

        // Zig-Zag (Right Left)
        // recursively bring val as root of right-left
        node->right->left = splay (node->right->left, val);

        return (node->right->left) ? zigzag(node) : zag(node);
    }
}


SplayNode* SplayTree::join(SplayNode* left, SplayNode* right)
{
    if(!left) return right;
    if(!right) return left;
    left = splay(left, right->value);
    left->right = right;
    return left;
}


// left <= val, val < right
pair<SplayNode*, SplayNode*> SplayTree::split(SplayNode* x, int val)
{
    if(!x) return {NULL, NULL};
    x = splay(x, val);
    if(x->value <= val)
    {
        SplayNode* right = x->right;
        x->right = NULL;
        return {x, right};
    }
    SplayNode* left = x->left;
    x->left = NULL;
    return {left, x};
}


bool SplayTree::search(int val)
{
    if(isEmpty()) return false;

    root = splay(root, val);
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
    pair<SplayNode*, SplayNode*> p;

    if(isEmpty())
    {
        root = new SplayNode(val);
        return;
    }
    root = splay(root, val);
    if(root->value == val)
        return;

    p = split(root, val);
    root = new SplayNode(val);
    root->left = p.first;
    root->right = p.second;
}


void SplayTree::remove(int val)
{
    SplayNode* rem_node = NULL;

    if(isEmpty()) return;

    root = splay (root, val);
    if(root->value != val)
        return;

    rem_node = root;
    root = join(root->left, root->right);
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
