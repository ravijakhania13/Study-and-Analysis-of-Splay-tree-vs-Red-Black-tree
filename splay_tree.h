#ifndef _SPLAY_TREE_H_
#define _SPLAY_TREE_H_

#include <iostream>

struct SplayNode
{
    int value;
    struct SplayNode *left;
    struct SplayNode *right;

    SplayNode(int v = -1): value(v), left(NULL), right(NULL) {}
};

enum class Splaying { TOP_DOWN, BOTTOM_UP };

class SplayTree
{
    SplayNode *root;
    Splaying splaying;
    int num_comparisons;
    int num_rotations;

    SplayNode*                    splay(SplayNode*, int);
    SplayNode*                    splayBottomUp(SplayNode*, int);
    SplayNode*                    splayTopDown (SplayNode*, int);
    SplayNode*                    join (SplayNode*, SplayNode*);
    std::pair<SplayNode*, SplayNode*>  split(SplayNode*, int);
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

    double num_del_comps;
    double num_del_rots;
    double num_ins_comps;
    double num_ins_rots;
    double num_search_comps;
    double num_search_rots;

    SplayTree(Splaying s = Splaying::TOP_DOWN): splaying(s), root(NULL),
                                                num_comparisons(0), num_rotations(0),
                                                num_del_comps(0), num_del_rots(0),
                                                num_ins_comps(0), num_ins_rots(0),
                                                num_search_comps(0), num_search_rots(0) {}

    void insert(int);
    bool search(int);
    void remove(int);

    void preorder() { preorderUtil(root); std::cout << std::endl; }
    void inorder() { inorderUtil(root); std::cout << std::endl; }

    bool isEmpty() { return root == NULL; }
    void clear() { clearUtil(root); root = NULL; }

    void numComparisonsInit() { num_comparisons = 0; }
    int numComparisonsGet() { return num_comparisons; }
    void numRotationsInit() { num_rotations = 0; }
    int numRotationsGet() { return num_rotations; }
    void readingsInit() { num_comparisons = num_rotations = 
                          num_del_comps = num_del_rots = 
                          num_ins_comps = num_ins_rots = 
                          num_search_comps = num_search_rots = 0; }

    ~SplayTree()
    {
        clear();
    }
};

#endif  // _SPLAY_TREE_H_
