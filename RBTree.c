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

void rotateLeft(Node *pivot)
{
    Node *son = pivot->right;
    Node *parent = pivot->parent;
    if (parent == NULL)
    {
        return;
    }
    pivot->right = son->left;
    son->left = pivot;
    pivot->parent = son;
    if (pivot->right != NULL)
    {
        pivot->right->parent = pivot;
    }

    if (parent != NULL)
    {
        if (pivot == parent->left)
        {
            pivot->left = son;
        }
        else if (pivot == parent->right)
        {
            parent->right = son;
        }
    }
    son->parent = parent;
}

void rotateRight(Node *pivot)
{
    Node *son = pivot->left;
    Node *parent = pivot->parent;
    if (son == NULL)
    {
        return;
    }
    pivot->left = son->right;
    son->right = pivot;
    pivot->parent = son;
    if (pivot->left != NULL)
    {
        pivot->left->parent = pivot;
    }
    if (parent != NULL)
    {
        if (pivot == parent->left)
        {
            parent->left = son;
        }
        else if (pivot == parent->right)
        {
            parent->right = son;
        }
    }
    son->parent = parent;
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
        rotateLeft(father);
        node = node->left;
    }
    else if (node == father->left && father == grandpa->right)
    {
        rotateRight(father);
        node = node->right;
    }

    //4 2
    if (node == father->left)
    {
        rotateRight(grandpa);
    }
    else
    {
        rotateLeft(grandpa);
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
    Node *father = findFatherOfData(tree, data);
    if (father == NULL)
    {
        node->color = BLACK;
        tree->root = node;
        tree->size += 1;
        return true;
    }
    // insert
    if (tree->compFunc(father->data, node->data) < 0)
    {
        father->right = node;
    }
    else
    {
        father->left = node;
    }
    node->parent = father;

    fixInsert(tree, node);
    tree->size += 1;
    return true;
}

void swapValues(Node *node1, Node *node2)
{
    int temp = node1->data;
    node1->data = node2->data;
    node2->data = temp;
}

void swapColors(Node *node1, Node *node2)
{
    int temp = node1->color;
    node1->data = node2->color;
    node2->data = temp;
}

Node *getBrother(Node *node)
{
    Node *father = node->parent;
    if (father == NULL)
    {
        return NULL;
    }
    if (father->left == node)
    {
        return father->right;
    }
    return father->left;
}

void delCase6(Node *node);

void delCase5(Node *node);

void delCase4(Node *node);

void delCase3(Node *node)
{
    Node *brother = getBrother(node);
    if ((node->parent->color == BLACK) && ((brother == NULL) || brother->color == BLACK)
        && ((brother->left == NULL) || (brother->left->color == BLACK))
        && ((brother->right == NULL) || (brother->right->color == BLACK)))
    {
        brother->color = RED;
        delCase1(node);
    }
    else
    {
        delCase4(node);
    }
}

void delCase2(Node *node)
{
    Node *brother = getBrother(node);
    if (brother == NULL)
    {
        return;
    }

    if (brother->color == RED)
    {
        node->parent->color = RED;
        brother->color = BLACK;
        if (node  == node->parent->left)
        {
            rotateLeft(node->parent);
        }
        else
        {
            rotateRight(node->parent);
        }
    }
    delCase3(node);
}

void delCase1(Node *node)
{
    if (node->parent != NULL)
    {
        delCase2(node);
    }
}

void replaceNode(Node *node, Node *child)
{
    child->parent = node->parent;
    if (node == node->parent->left)
    {
        node->parent->left = child;
    }
    else
    {
        node->parent->right = child;
    }
}

void deleteChild(Node *node)
{
    Node *child = NULL;
    if (node->right != NULL)
    {
        child = node->right;
    }
    else if (node->left != NULL)
    {
        child = node->left;
    }
    else
    {
        return;
    }

    replaceNode(node, child);
    if (node->color == BLACK)
    {
        if (child->color == RED)
        {
             child->color = BLACK;
        }
        else
        {
            delCase1(child);
        }
    }
    free(node);
}

int deleteFromRBTree(RBTree *tree, void *data)
{
    if (!RBTreeContains(tree, data))
    {
        return false;
    }

}