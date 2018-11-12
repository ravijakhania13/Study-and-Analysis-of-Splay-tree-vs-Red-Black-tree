/*
 * C++ Program to Implement Splay Tree
 */

#include <iostream>
#include "timer.h"
#include "splay_tree.h"

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
    // Increment the number of rotations
    ++num_rotations;

    SplayNode* l = node->left;
    node->left = l->right;
    l->right = node;
    return l;
}

// Left Rotate
SplayNode* SplayTree::zag(SplayNode *node)
{
    // Increment the number of rotations
    ++num_rotations;

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

SplayNode* SplayTree::splayBottomUp(SplayNode* node, int val)
{
    /* Increment the number of comparisons
       i.e. the number of nodes visited */
    ++num_comparisons;

    // Base cases: node is NULL or val is present at node
    if(!node || node->value == val)
        return node;

    ++num_comparisons;
    // val lies in left subtree
    if(node->value > val)
    {
        // Key is not in tree
        if(!node->left) return node;

        /* Increment the number of comparisons
           i.e. the number of nodes visited */
        ++num_comparisons;
        if(node->left->value == val)
        {
            return zig(node);
        }

        ++num_comparisons;
        // Zig-Zig (Left Left)
        if(node->left->value > val)
        {
            // recursively bring val as root of left-left
            node->left->left = splayBottomUp (node->left->left, val);
            return (node->left->left) ? zigzig(node) : zig(node);
        }

        // Zag-Zig (Left Right)
        // recursively bring val as root of left-right
        node->left->right = splayBottomUp (node->left->right, val);
        return (node->left->right) ? zagzig(node) : zig(node);
    }
    else    // val lies in right subtree
    {
        // val is not in tree
        if(!node->right) return node;

        /* Increment the number of comparisons
           i.e. the number of nodes visited */
        ++num_comparisons;
        if (node->right->value == val)
        {
            return zag (node);
        }

        ++num_comparisons;
        // Zag-Zag (Right Right)
        if(node->right->value < val)
        {
            // recursively bring val as root of right-right
            node->right->right = splayBottomUp (node->right->right, val);
            return (node->right->right) ? zagzag(node) : zag(node);
        }

        // Zig-Zag (Right Left)
        // recursively bring val as root of right-left
        node->right->left = splayBottomUp (node->right->left, val);
        return (node->right->left) ? zigzag(node) : zag(node);
    }
}


SplayNode* SplayTree::splayTopDown(SplayNode* node, int val)
{
    if (!node) return NULL;

    /* aux_node.right points to R tree;
       aux_node.left  points to L Tree */
    SplayNode aux_node;
    SplayNode* left_max_node = &aux_node;
    SplayNode* right_min_node = &aux_node;

    while(1)
    {
        /* Increment the number of comparisons
           i.e. the number of nodes visited */
        ++num_comparisons;
        if (val == node->value)
            break;

        ++num_comparisons;
        if(val < node->value)
        {
            if(!node->left)
                break;

            /* Increment the number of comparisons
               i.e. the number of nodes visited */
            ++num_comparisons;
            if(val < node->left->value)
            {
                // only zig-zig case needs to rotate once
                node = zig(node);
                if (!node->left)
                    break;
            }
            /* Link to R Tree */
            right_min_node->left = node;
            right_min_node = right_min_node->left;
            node = node->left;
            right_min_node->left = NULL;
        }
        else
        {
            if(!node->right)
                break;
            /* Increment the number of comparisons
               i.e. the number of nodes visited */
            ++num_comparisons;
            if(val > node->right->value)
            {
                // only zag-zag mode need to rotate once
                node = zag(node);
                if(!node->right)
                    break;
            }
            /* Link to L Tree */
            left_max_node->right = node;
            left_max_node = left_max_node->right;
            node = node->right;
            left_max_node->right = NULL;
        }
    }
    /* assemble L Tree, Middle Tree and R tree */
    left_max_node->right = node->left;
    right_min_node->left = node->right;
    node->left = aux_node.right;
    node->right = aux_node.left;
    return node;
}

SplayNode* SplayTree::splay(SplayNode* node, int val)
{
    num_comparisons = num_rotations = 0;
    if (splaying == Splaying::BOTTOM_UP)
        return splayBottomUp(node, val);
    else
        return splayTopDown(node, val);
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
std::pair<SplayNode*, SplayNode*> SplayTree::split(SplayNode* x, int val)
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

    bool ret = true;
    root = splay(root, val);

    // add rotations and comparisons
    num_search_comps += num_comparisons;
    num_search_rots += num_rotations;

    if (root->value != val)
        ret = false;

    return ret;
}

void SplayTree::insert(int val)
{
    std::pair<SplayNode*, SplayNode*> p;

    if(isEmpty())
    {
        root = new SplayNode(val);
        return;
    }
    p = split(root, val);

    // add rotations and comparisons
    num_ins_comps += num_comparisons;
    num_ins_rots += num_rotations;

    if(p.first && p.first->value == val)
    {
        root = p.first;
        root->right = p.second;
        return;
    }

    root = new SplayNode(val);
    root->left = p.first;
    root->right = p.second;
}


void SplayTree::remove(int val)
{
    SplayNode* rem_node = NULL;

    if(isEmpty()) return;

    root = splay (root, val);

    // add rotations and comparisons
    num_del_comps += num_comparisons;
    num_del_rots += num_rotations;

    if(root->value != val)
        return;

    rem_node = root;
    root = join(root->left, root->right);

    // add rotations and comparisons as join() also calls splay()
    num_del_comps += num_comparisons;
    num_del_rots += num_rotations;

    delete rem_node;
    rem_node = NULL;
}


void SplayTree::inorderUtil(SplayNode* node)
{
    if(node)
    {
        inorderUtil(node->left);
        std::cout << node->value << " ";
        inorderUtil(node->right);
    }
}


void SplayTree::preorderUtil(SplayNode* node)
{
    if(node)
    {
        std::cout << node->value << " ";
        preorderUtil(node->left);
        preorderUtil(node->right);
    }
}
