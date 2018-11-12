#ifndef _RED_BLACK_TREE_H_
#define _RED_BLACK_TREE_H_

#include <iostream>

enum class Color { RED, BLACK };

// RBTree Node Declaration
struct RBTreeNode
{
    Color color;
    int value;
    RBTreeNode *left, *right, *parent;

    RBTreeNode (int v, Color c, RBTreeNode* l = NULL, RBTreeNode* r = NULL): color(c), value(v), left(l), right(r) 
    {
        if (left) left->parent = this;
        if (right) right->parent = this;
        parent = NULL;
    }
};

// Class RBTree Declaration
class RBTree
{
    RBTreeNode* root;
    int num_comparisons;
    int num_rotations;

    RBTreeNode* grandparent(RBTreeNode*);
    RBTreeNode* sibling(RBTreeNode*);
    RBTreeNode* uncle(RBTreeNode*);
    RBTreeNode* maximumNode(RBTreeNode*);
    RBTreeNode* searchNode(int);
    Color       nodeColor(RBTreeNode*);

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

    double num_del_comps;
    double num_del_rots;
    double num_ins_comps;
    double num_ins_rots;
    double num_search_comps;
    double num_search_rots;

    RBTree(): root(NULL),
              num_comparisons(0), num_rotations(0),
              num_del_comps(0), num_del_rots(0),
              num_ins_comps(0), num_ins_rots(0),
              num_search_comps(0), num_search_rots(0) {}

    bool search(int);
    void insert(int);
    void remove(int);

    void clear() { clearUtil(root); root = NULL; }
    void print();

    void readingsInit() { num_comparisons = num_rotations =
                          num_del_comps = num_del_rots =
                          num_ins_comps = num_ins_rots =
                          num_search_comps = num_search_rots = 0; }


    ~RBTree()
    {
        clear();
    }
};

#endif  // _RED_BLACK_TREE_H_
