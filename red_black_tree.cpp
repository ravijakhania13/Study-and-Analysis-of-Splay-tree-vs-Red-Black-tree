/*
 * C++ Program to Implement Red Black Tree
 */
#include <iostream>
#include <cassert>
#include <random>
#include <chrono>
#include <time.h>
#include <fstream>

#define INDENT_STEP  4

using namespace std;

enum color { RED, BLACK };

/*
 * RBTree Node Declaration
 */
struct RBTreeNode
{
    enum color color;
    int value;
    RBTreeNode *left, *right, *parent;

    RBTreeNode (int v, enum color c, RBTreeNode* l = NULL, RBTreeNode* r = NULL): color(c), value(v), left(l), right(r) 
    {
        if (left) left->parent = this;
        if (right) right->parent = this;
        parent = NULL;
    }
};

/*
 * Class RBTree Declaration
 */
class RBTree
{
    RBTreeNode* root;

    RBTreeNode* grandparent(RBTreeNode*);
    RBTreeNode* sibling(RBTreeNode*);
    RBTreeNode* uncle(RBTreeNode*);
    RBTreeNode* maximumNode(RBTreeNode*);
    RBTreeNode* searchNode(int);
    color       nodeColor(RBTreeNode*);

    void rotateLeft(RBTreeNode*);
    void rotateRight(RBTreeNode*);
    void replaceNode(RBTreeNode* oldn, RBTreeNode* newn);

    void insertCase1(RBTreeNode*);
    void insertCase2(RBTreeNode*);
    void insertCase3(RBTreeNode*);
    void insertCase4(RBTreeNode*);
    void insertCase5(RBTreeNode*);

    void removeCase1(RBTreeNode*);
    void removeCase2(RBTreeNode*);
    void removeCase3(RBTreeNode*);
    void removeCase4(RBTreeNode*);
    void removeCase5(RBTreeNode*);
    void removeCase6(RBTreeNode*);

    void clearUtil(RBTreeNode*);
    void print_helper(RBTreeNode*, int);

public:

    RBTree(): root(NULL) {}

    bool search(int);
    void insert(int);
    void remove(int);

    void clear() { clearUtil(root); root = NULL; }
    void print();

    ~RBTree()
    {
        clear();
    }

};


void RBTree::clearUtil(RBTreeNode* node)
{
    if(!node) return;

    clearUtil(node->left);
    clearUtil(node->right);
    delete node;
}


/*
 * Return Grandparent of Node
 */
RBTreeNode* RBTree::grandparent(RBTreeNode* n)
{
    assert (n != NULL);
    assert (n->parent != NULL);
    assert (n->parent->parent != NULL);
    return n->parent->parent;
}

/*
 * Return Sibling of Node
 */
RBTreeNode* RBTree::sibling(RBTreeNode* n)
{
    assert (n != NULL);
    assert (n->parent != NULL);
    if (n == n->parent->left)
        return n->parent->right;
    else
        return n->parent->left;
}

/*
 * Return Uncle of Node
 */
RBTreeNode* RBTree::uncle(RBTreeNode* n)
{
    assert (n != NULL);
    assert (n->parent != NULL);
    assert (n->parent->parent != NULL);
    return sibling(n->parent);
}


/*
 * Returns color of a RBTreeNode*
 */
color RBTree::nodeColor(RBTreeNode* n)
{
    return n == NULL ? BLACK : n->color;
}

/*
 * RbTree Search
 */
bool RBTree::search(int value)
{
    RBTreeNode* n = searchNode(value);
    return (n != NULL);
}


/*
 * RbTree Search Node
 */
RBTreeNode* RBTree::searchNode(int value)
{
    RBTreeNode* n = root;
    while (n)
    {
        if (n->value == value)
            break;

        if (value < n->value)
        {
            n = n->left;
        }
        else
        {
            assert(value > n->value);
            n = n->right;
        }
    }
    return n;
}



/*
 * Rotate left
 */
void RBTree::rotateLeft(RBTreeNode* n)
{
    RBTreeNode* r = n->right;
    replaceNode(n, r);
    n->right = r->left;
    if (r->left)
    {
        r->left->parent = n;
    }
    r->left = n;
    n->parent = r;
}

/*
 * Rotate right
 */
void RBTree::rotateRight(RBTreeNode* n)
{
    RBTreeNode* l = n->left;
    replaceNode(n, l);
    n->left = l->right;
    if (l->right)
    {
        l->right->parent = n;
    }
    l->right = n;
    n->parent = l;
}

/*
 * Replace a RBTreeNode*
 */
void RBTree::replaceNode(RBTreeNode* oldn, RBTreeNode* newn)
{
    if (oldn->parent == NULL)
    {
        root = newn;
    }
    else
    {
        if (oldn == oldn->parent->left)
            oldn->parent->left = newn;
        else
            oldn->parent->right = newn;
    }
    if (newn != NULL)
    {
        newn->parent = oldn->parent;
    }
}

/*
 * Insert RBTreeNode* into RBTree
 */
void RBTree::insert(int value)
{
    RBTreeNode* new_node = new RBTreeNode(value, RED);

    if (!root)
    {
        root = new_node;
    }
    else
    {
        RBTreeNode* n = root;
        while (1)
        {
            if (value == n->value)
            {
                delete new_node;
                new_node = NULL;
                return;
            }

            if (value < n->value)
            {
                if (!n->left)
                {
                    n->left = new_node;
                    break;
                }
                else
                {
                    n = n->left;
                }
            }
            else
            {
                assert (value > n->value);
                if (!n->right)
                {
                    n->right = new_node;
                    break;
                }
                else
                {
                    n = n->right;
                }
            }
        }
        new_node->parent = n;
    }
    insertCase1(new_node);
}


/*
 * Inserting Case 1
 */
void RBTree::insertCase1(RBTreeNode* n)
{
    if (n->parent == NULL)
        n->color = BLACK;
    else
        insertCase2(n);
}


/*
 * Inserting Case 2
 */
void RBTree::insertCase2(RBTreeNode* n)
{
    if (nodeColor(n->parent) == BLACK)
        return;
    else
        insertCase3(n);
}


/*
 * Inserting Case 3
 */
void RBTree::insertCase3(RBTreeNode* n)
{
    if (nodeColor(uncle(n)) == RED)
    {
        n->parent->color = BLACK;
        uncle(n)->color = BLACK;
        grandparent(n)->color = RED;
        insertCase1(grandparent(n));
    }
    else
    {
        insertCase4(n);
    }
}

/*
 * Inserting Case 4
 */
void RBTree::insertCase4(RBTreeNode* n)
{
    if (n == n->parent->right && n->parent == grandparent(n)->left)
    {
        rotateLeft(n->parent);
        n = n->left;
    }
    else if (n == n->parent->left && n->parent == grandparent(n)->right)
    {
        rotateRight(n->parent);
        n = n->right;
    }
    insertCase5(n);
}

/*
 * Inserting Case 5
 */
void RBTree::insertCase5(RBTreeNode* n)
{
    n->parent->color = BLACK;
    grandparent(n)->color = RED;
    if (n == n->parent->left && n->parent == grandparent(n)->left)
    {
        rotateRight(grandparent(n));
    }
    else
    {
        assert (n == n->parent->right && n->parent == grandparent(n)->right);
        rotateLeft(grandparent(n));
    }
}

/*
 * Delete Node from RBTree
 */
void RBTree::remove(int value)
{
    RBTreeNode* child;
    RBTreeNode* n = searchNode(value);

    if (!n) return;

    if (n->left && n->right)
    {
        RBTreeNode* pred = maximumNode(n->left);
        n->value = pred->value;
        n = pred;
    }

    assert(n->left == NULL || n->right == NULL);
    child = n->right == NULL ? n->left  : n->right;

    if (nodeColor(n) == BLACK)
    {
        n->color = nodeColor(child);
        removeCase1(n);
    }
    replaceNode(n, child);

    delete n;
    n = NULL;

    if(root) root->color = BLACK;
}

/*
 * Returns Maximum RBTreeNode*
 */
RBTreeNode* RBTree::maximumNode(RBTreeNode* n)
{
    assert (n != NULL);
    while (n->right)
    {
        n = n->right;
    }
    return n;
}

/*
 * Deleting Case 1
 */
void RBTree::removeCase1(RBTreeNode* n)
{
    if (!n->parent)
        return;
    else
        removeCase2(n);
}

/*
 * Deleting Case 2
 */
void RBTree::removeCase2(RBTreeNode* n)
{
    if (nodeColor(sibling(n)) == RED)
    {
        n->parent->color = RED;
        sibling(n)->color = BLACK;
        if (n == n->parent->left)
            rotateLeft(n->parent);
        else
            rotateRight(n->parent);
    }
    removeCase3(n);
}

/*
 * Deleting Case 3
 */
void RBTree::removeCase3(RBTreeNode* n)
{
    if (nodeColor(n->parent) == BLACK && nodeColor(sibling(n)) == BLACK &&
        nodeColor(sibling(n)->left) == BLACK && nodeColor(sibling(n)->right) == BLACK)
    {
        sibling(n)->color = RED;
        removeCase1(n->parent);
    }
    else
        removeCase4(n);
}

/*
 * Deleting Case 4
 */
void RBTree::removeCase4(RBTreeNode* n)
{
    if (nodeColor(n->parent) == RED && nodeColor(sibling(n)) == BLACK &&
        nodeColor(sibling(n)->left) == BLACK && nodeColor(sibling(n)->right) == BLACK)
    {
        sibling(n)->color = RED;
        n->parent->color = BLACK;
    }
    else
        removeCase5(n);
}

/*
 * Deleting Case 5
 */
void RBTree::removeCase5(RBTreeNode* n)
{
    if (n == n->parent->left && nodeColor(sibling(n)) == BLACK &&
        nodeColor(sibling(n)->left) == RED && nodeColor(sibling(n)->right) == BLACK)
    {
        sibling(n)->color = RED;
        sibling(n)->left->color = BLACK;
        rotateRight(sibling(n));
    }
    else if (n == n->parent->right && nodeColor(sibling(n)) == BLACK &&
             nodeColor(sibling(n)->right) == RED && nodeColor(sibling(n)->left) == BLACK)
    {
        sibling(n)->color = RED;
        sibling(n)->right->color = BLACK;
        rotateLeft(sibling(n));
    }
    removeCase6(n);
}

/*
 * Deleting Case 6
 */
void RBTree::removeCase6(RBTreeNode* n)
{
    sibling(n)->color = nodeColor(n->parent);
    n->parent->color = BLACK;
    if (n == n->parent->left)
    {
        assert (nodeColor(sibling(n)->right) == RED);
        sibling(n)->right->color = BLACK;
        rotateLeft(n->parent);
    }
    else
    {
        assert (nodeColor(sibling(n)->left) == RED);
        sibling(n)->left->color = BLACK;
        rotateRight(n->parent);
    }
}


/*
 * Print RBTRee
 */
void RBTree::print_helper(RBTreeNode* n, int indent)
{
    if (!n)
    {
        return;
    }

    print_helper(n->right, indent + INDENT_STEP);

    for(int i = 0; i < indent; i++)
        cout << " ";

    if (n->color == BLACK)
        cout << n->value << endl;
    else
        cout << "<" << n->value << ">" << endl;

    print_helper(n->left, indent + INDENT_STEP);
}

void RBTree::print()
{
    if(!root)
    {
        cout << "<empty tree>" << endl;
        return;
    }
    print_helper(root, 0);
    cout << endl;
}


/*
 * Main Contains Menu
 */
int main()
{
    double avg_insertion[5]={0,0};
    double avg_searching[5]={0,0};
    double avg_deletion[5]={0,0};

    clock_t start, end;
    for (int k = 0 ; k < 10 ; k ++)
    {
        int j = 0;
        for (int i = 100 ; i <= 1000000 ; i *= 10, ++j)
        {
            RBTree rbt;

            random_device rd;   // obtain a random number from hardware
            mt19937 eng(rd());  // seed the generator
            uniform_int_distribution<> distr(1, i); // define the range

            cout << "i = " << i << endl;
            start = clock();
            for(int n = 0; n < 10000000; ++n)
                rbt.insert(distr(eng));
            end = clock();

            avg_insertion[j] += ((double) (end - start)) / CLOCKS_PER_SEC;


            random_device rd1;   // obtain a random number from hardware
            mt19937 eng1(rd1()); // seed the generator
            uniform_int_distribution<> distr1(i-100, i); // define the range

            start = clock();
            for(int n=0; n<1000000; ++n)
                rbt.search(distr1(eng1));
            end = clock();
            avg_searching[j] += ((double) (end - start)) / CLOCKS_PER_SEC;


            random_device rd2;     // obtain a random number from hardware
            mt19937 eng2(rd2());   // seed the generator
            uniform_int_distribution<> distr2(1, i); // define the range

            start = clock();
            for(int n=0; n<1000000; ++n)
                rbt.remove(distr2(eng2));
            end = clock();
            avg_deletion[j] += ((double) (end - start)) / CLOCKS_PER_SEC;
        }
    }

    ofstream myfile1,myfile2,myfile3;
    myfile1.open ("RBT_insert.txt");
    myfile2.open ("RBT_search.txt");
    myfile3.open ("RBT_delete.txt");

    for(int i = 0 ; i < 5 ; i++)
    {
        avg_insertion[i] /= 10;
        avg_searching[i] /= 10;
        avg_deletion[i] /= 10;

        myfile1 << avg_insertion[i] << "\n";
        myfile2 << avg_searching[i] << "\n";
        myfile3 << avg_deletion[i] << "\n";
        cout<< avg_insertion[i] << "\t" << avg_searching[i] << "\t" << avg_deletion[i]<<endl;
    }

    return 0;
}
