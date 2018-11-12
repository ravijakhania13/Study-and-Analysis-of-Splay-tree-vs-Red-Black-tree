/*
 * C++ Program to Implement Red Black Tree
 */
//#include <cassert>
#include "red_black_tree.h"

#define INDENT_STEP  4

void RBTree::clearUtil(RBTreeNode* node)
{
    if(!node) return;

    clearUtil(node->left);
    clearUtil(node->right);
    delete node;
}

// Return Grandparent of Node
RBTreeNode* RBTree::grandparent(RBTreeNode* n)
{
    //assert (n != NULL);
    //assert (n->parent != NULL);
    //assert (n->parent->parent != NULL);
    return n->parent->parent;
}

// Return Sibling of Node
RBTreeNode* RBTree::sibling(RBTreeNode* n)
{
    //assert (n != NULL);
    //assert (n->parent != NULL);
    if (n == n->parent->left)
        return n->parent->right;
    else
        return n->parent->left;
}

// Return Uncle of Node
RBTreeNode* RBTree::uncle(RBTreeNode* n)
{
    //assert (n != NULL);
    //assert (n->parent != NULL);
    //assert (n->parent->parent != NULL);
    return sibling(n->parent);
}


// Returns color of a RBTreeNode*
Color RBTree::nodeColor(RBTreeNode* n)
{
    return n == NULL ? Color::BLACK : n->color;
}

// RbTree Search
bool RBTree::search(int value)
{
    num_comparisons = 0;
    RBTreeNode* n = searchNode(value);

    num_search_comps += num_comparisons;
    return (n != NULL);
}


// RbTree Search Node
RBTreeNode* RBTree::searchNode(int value)
{
    RBTreeNode* n = root;
    while (n)
    {
        /* Increment the number of comparisons
           i.e. the number of nodes visited */
        ++num_comparisons;
        if (n->value == value)
            break;

        ++num_comparisons;
        if (value < n->value)
        {
            n = n->left;
        }
        else
        {
            //assert(value > n->value);
            n = n->right;
        }
    }
    return n;
}


// Rotate left
void RBTree::rotateLeft(RBTreeNode* n)
{
    ++num_rotations;

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

// Rotate right
void RBTree::rotateRight(RBTreeNode* n)
{
    ++num_rotations;

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

// Replace a RBTreeNode*
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

// Insert RBTreeNode* into RBTree
void RBTree::insert(int value)
{
    RBTreeNode* new_node = new RBTreeNode(value, Color::RED);

    num_comparisons = num_rotations = 0;
    if (!root)
    {
        root = new_node;
    }
    else
    {
        RBTreeNode* n = root;
        while (1)
        {
            /* Increment the number of comparisons
               i.e. the number of nodes visited */
            ++num_comparisons;
            if (value == n->value)
            {
                delete new_node;
                new_node = NULL;
                return;
            }

            ++num_comparisons;
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
                //assert (value > n->value);
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
    num_ins_comps += num_comparisons;
    num_ins_rots += num_rotations;
}


// Inserting Case 1
void RBTree::insertCase1(RBTreeNode* n)
{
    if (n->parent == NULL)
        n->color = Color::BLACK;
    else
        insertCase2(n);
}


// Inserting Case 2
void RBTree::insertCase2(RBTreeNode* n)
{
    if (nodeColor(n->parent) == Color::BLACK)
    {
        return;
    }
    else
        insertCase3(n);
}


// Inserting Case 3
void RBTree::insertCase3(RBTreeNode* n)
{
    if (nodeColor(uncle(n)) == Color::RED)
    {
        n->parent->color = Color::BLACK;
        uncle(n)->color = Color::BLACK;
        grandparent(n)->color = Color::RED;

        insertCase1(grandparent(n));
    }
    else
    {
        insertCase4(n);
    }
}

// Inserting Case 4
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

// Inserting Case 5
void RBTree::insertCase5(RBTreeNode* n)
{
    n->parent->color = Color::BLACK;
    grandparent(n)->color = Color::RED;
    if (n == n->parent->left && n->parent == grandparent(n)->left)
    {
        rotateRight(grandparent(n));
    }
    else
    {
        //assert (n == n->parent->right && n->parent == grandparent(n)->right);
        rotateLeft(grandparent(n));
    }
}

// Delete Node from RBTree
void RBTree::remove(int value)
{
    RBTreeNode* child;
    num_comparisons = num_rotations = 0;

    RBTreeNode* n = searchNode(value);

    if (!n) return;

    if (n->left && n->right)
    {
        RBTreeNode* pred = maximumNode(n->left);
        n->value = pred->value;
        n = pred;
    }

    //assert(n->left == NULL || n->right == NULL);
    child = n->right == NULL ? n->left  : n->right;

    if (nodeColor(n) == Color::BLACK)
    {
        n->color = nodeColor(child);
        removeCase1(n);
    }
    replaceNode(n, child);

    delete n;
    n = NULL;

    if(root) root->color = Color::BLACK;

    num_del_comps += num_comparisons;
    num_del_rots += num_rotations;
}

// Returns Maximum RBTreeNode*
RBTreeNode* RBTree::maximumNode(RBTreeNode* n)
{
    //assert (n != NULL);
    while (n->right)
    {
        n = n->right;
    }
    return n;
}

// Deleting Case 1
void RBTree::removeCase1(RBTreeNode* n)
{
    if (!n->parent)
        return;
    else
        removeCase2(n);
}

// Deleting Case 2
void RBTree::removeCase2(RBTreeNode* n)
{
    if (nodeColor(sibling(n)) == Color::RED)
    {
        n->parent->color = Color::RED;
        sibling(n)->color = Color::BLACK;
        if (n == n->parent->left)
            rotateLeft(n->parent);
        else
            rotateRight(n->parent);
    }
    removeCase3(n);
}

// Deleting Case 3
void RBTree::removeCase3(RBTreeNode* n)
{
    if (nodeColor(n->parent) == Color::BLACK && nodeColor(sibling(n)) == Color::BLACK &&
        nodeColor(sibling(n)->left) == Color::BLACK && nodeColor(sibling(n)->right) == Color::BLACK)
    {
        sibling(n)->color = Color::RED;
        removeCase1(n->parent);
    }
    else
        removeCase4(n);
}

// Deleting Case 4
void RBTree::removeCase4(RBTreeNode* n)
{
    if (nodeColor(n->parent) == Color::RED && nodeColor(sibling(n)) == Color::BLACK &&
        nodeColor(sibling(n)->left) == Color::BLACK && nodeColor(sibling(n)->right) == Color::BLACK)
    {
        sibling(n)->color = Color::RED;
        n->parent->color = Color::BLACK;
    }
    else
        removeCase5(n);
}

// Deleting Case 5
void RBTree::removeCase5(RBTreeNode* n)
{
    if (n == n->parent->left && nodeColor(sibling(n)) == Color::BLACK &&
        nodeColor(sibling(n)->left) == Color::RED && nodeColor(sibling(n)->right) == Color::BLACK)
    {
        sibling(n)->color = Color::RED;
        sibling(n)->left->color = Color::BLACK;
        rotateRight(sibling(n));
    }
    else if (n == n->parent->right && nodeColor(sibling(n)) == Color::BLACK &&
             nodeColor(sibling(n)->right) == Color::RED && nodeColor(sibling(n)->left) == Color::BLACK)
    {
        sibling(n)->color = Color::RED;
        sibling(n)->right->color = Color::BLACK;
        rotateLeft(sibling(n));
    }
    removeCase6(n);
}

// Deleting Case 6
void RBTree::removeCase6(RBTreeNode* n)
{
    sibling(n)->color = nodeColor(n->parent);
    n->parent->color = Color::BLACK;
    if (n == n->parent->left)
    {
        //assert (nodeColor(sibling(n)->right) == Color::RED);
        sibling(n)->right->color = Color::BLACK;
        rotateLeft(n->parent);
    }
    else
    {
        //assert (nodeColor(sibling(n)->left) == Color::RED);
        sibling(n)->left->color = Color::BLACK;
        rotateRight(n->parent);
    }
}


// Print RBTRee
void RBTree::print_helper(RBTreeNode* n, int indent)
{
    if (!n)
    {
        return;
    }

    print_helper(n->right, indent + INDENT_STEP);

    for(int i = 0; i < indent; i++)
        std::cout << " ";

    if (n->color == Color::BLACK)
        std::cout << n->value << std::endl;
    else
        std::cout << "<" << n->value << ">" << std::endl;

    print_helper(n->left, indent + INDENT_STEP);
}

void RBTree::print()
{
    if(!root)
    {
        std::cout << "<empty tree>" << std::endl;
        return;
    }
    print_helper(root, 0);
    std::cout << std::endl;
}
