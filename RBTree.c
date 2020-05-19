//
// Created by User on 19/05/2020.
//
#include "RBTree.h"
#include <stdlib.h>
#include "stdbool.h"

RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
    RBTree *tree = (RBTree *)calloc(1, sizeof(RBTree));
    if (tree == NULL || compFunc == NULL || freeFunc == NULL)
    {
        return NULL;
    }
    tree->freeFunc = freeFunc;
    tree->compFunc = compFunc;
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

int RBTreeContains(const RBTree *tree, const void *data)
{
    Node *root = tree->root;
    while(root != NULL)
    {
        int comp = tree->compFunc(root->data, data);
        if (comp == 0)
        {
            return true;
        }
        if (comp < 0)
        {
            root = root->left;
        }
        else
        {
            root = root->right;
        }
    }
    return false;
}

Node *CreateNode(void *data)
{
    Node *node = (Node *)calloc(1, sizeof(Node));
    if (node == NULL)
    {
        return NULL;
    }
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->color = RED;
    node->data = data;
    return node;
}

void rotateLeft(RBTree *tree, Node *pivot)
{
    Node *son = pivot->right;
    pivot->right = son->left;
    if (pivot->right != NULL)
    {
        pivot->right->parent = pivot;
    }
    son->parent = pivot->parent;
    if (pivot->parent == NULL)
    {
        tree->root = son;
    }
    else if (pivot == pivot->parent->left)
    {
        pivot->parent->left = son;
    }
    else
    {
        pivot->parent->right = son;
    }
    son->left = pivot;
    pivot->parent = son;
}

void rotateRight(RBTree *tree, Node *pivot)
{
    Node *son = pivot->left;
    pivot->left = son->right;
    if (pivot->left != NULL)
    {
        pivot->left->parent = pivot;
    }
    son->parent = pivot->parent;
    if (pivot->parent == NULL)
    {
        tree->root = son;
    }
    else if (pivot == pivot->parent->left)
    {
        pivot->parent->left = son;
    }
    else
    {
        pivot->parent->right = son;
    }
    son->right = pivot;
    pivot->parent = son;
}

/**
 * finds the parent of the data which we will be inserting
 * @param tree - the root of the tree
 * @param data the data to insert
 * @return The parent of the data
 */
Node *findFatherOfData(RBTree *tree, const void *data)
{
    if (tree == NULL)
    {
        return NULL;
    }
    Node *root = tree->root;
    while (root != NULL)
    {
        if (tree->compFunc(root->data, data) < 0)
        {
            if (root->right == NULL)
            {
                return root;
            }
            root = root->right;
        }
        else
        {
            if (root->left == NULL)
            {
                return root;
            }
            root = root->left;
        }
    }
    return NULL;
}

Node *getUncle(Node *son)
{
    Node *parent = son->parent;
    if (parent == NULL)
    {
        return NULL;
    }
    Node *grandpa = parent->parent;
    Node *uncle = NULL;
    if (grandpa != NULL)
    {
        if (grandpa->left == parent)
        {
            uncle = grandpa->right;
        }
        else
        {
            uncle = grandpa->left;
        }
    }
    return uncle;
}

void fixInsert(RBTree *tree, Node* node)
{
    Node *father = node->parent;
    if (father == NULL)
    {
        node->color = BLACK;
        tree->root = node;
        return;
    }
    if (father->color == BLACK)
    {
        return;
    }
    // case 3 - dad is red and uncle is red
    Node *uncle = getUncle(node);
    Node *grandpa = father->parent;
    if (uncle != NULL && uncle->color == RED)
    {
        father->color = BLACK;
        uncle->color = BLACK;
        grandpa->color = RED;
        fixInsert(tree, grandpa);
        return;
    }

    //case 4
    if (node == father->right && father == grandpa->left)
    {
        rotateLeft(tree, father);
        node = node->left;
    }
    else if (node == father->left && father == grandpa->right)
    {
        rotateRight(tree, father);
        node = node->right;
    }

    //4 2
    if (node == father->left)
    {
        rotateRight(tree, grandpa);
    }
    else
    {
        rotateLeft(tree, grandpa);
    }
    father->color = BLACK;
    grandpa->color = RED;
}

int insertToRBTree(RBTree *tree, void *data)
{
    if (RBTreeContains(tree, data) == true)
    {
        return false;
    }
    // Now we know that the data isn't in the tree
    // find the location to enter it
    Node *node = CreateNode(data);
    if (node == NULL)
    {
        return false;
    }
    Node *parent = findFatherOfData(tree, data);
    // insert node to parent
    node->parent = parent;
    if (tree->compFunc(parent->data, node->data) < 0)
    {
        parent->right = node;
    }
    else
    {
        parent->left = node;
    }

    fixInsert(tree, node);
}