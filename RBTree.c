//
// Created by User on 19/05/2020.
//
#include "RBTree.h"
#include <stdlib.h>
#include "stdbool.h"
/**
void delCase1(Node *M);
void delCase2(Node *M);
void delCase3(Node *M);
void delCase4(Node *M);
void delCase5(Node *M);
void delCase6(Node *M);
*/
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
            root = root->right;
        }
        else
        {
            root = root->left;
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
    void *temp = node1->data;
    node1->data = node2->data;
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

Node *getSon(Node *node)
{
    if (node->left != NULL)
    {
        return node->left;
    }
    if (node->right != NULL)
    {
        return node->right;
    }
    return NULL;
}

Node *precessor(Node *node)
{
    Node *pred = node->left;
    while (pred->right != NULL)
    {
        pred = pred->right;
    }
    return pred;
}

void freeNode(RBTree *tree, Node *M)
{
    tree->freeFunc(M->data);
    free(M);
}

Node *getNode(RBTree *tree, void *data)
{
    Node *root = tree->root;
    while (root != NULL)
    {
        if (tree->compFunc(root->data, data) == 0)
        {
            return root;
        }
        if (tree->compFunc(root->data, data) < 0)
        {
            root = root->right;
        }
        else
        {
            root = root->left;
        }
    }
}

void fixDelete(Node *M)
{
    if (M->parent == NULL)
    {
        return;
    }

    Node *S = getBrother(M);
    if (S->color == RED)
    {
        M->parent->color = RED;
        S->color = BLACK;
        if (M == M->parent->left)
        {
            rotateLeft(M->parent);
        }
        else
        {
            rotateRight(M->parent);
        }
    }
    S = getBrother(M);
    if (M->parent->color == BLACK && S->color == BLACK && (S->left == NULL || S->left->color == BLACK) && (S->right == NULL || S->right->color == BLACK))
    {
        S->color = RED;
        fixDelete(M->parent);
    }
    else if (M->parent->color == RED && S->color == BLACK && S->left->color == BLACK && S->right->color == BLACK)
    {
        S->color = RED;
        M->parent->color = BLACK;
    }
    else
    {
        if (M == M->parent->left && S->color == BLACK && (S->left != NULL && S->left->color == RED) && (S->right == NULL || S->right->color == BLACK))
        {
            S->color = RED;
            S->left->color = BLACK;
            rotateRight(S);
        }
        else if (M == M->parent->right && S->color == BLACK && S->right->color == RED && S->left->color == BLACK)
        {
            S->color = RED;
            S->right->color = BLACK;
            rotateLeft(S);
        }
        S->color = M->parent->color;
        M->parent->color = BLACK;
        if (M == M->parent->left)
        {
            if (S->left != NULL)
            {
                S->left->color = BLACK;
            }
            rotateRight(M->parent);
        }
    }
}

void swap(RBTree *tree, Node *C, Node *M)
{
    if (C->parent == NULL)
    {
        tree->root = M;
    }
    else
    {
        if (C == C->parent->left)
        {
            C->parent->left = M;
        }
        else
        {
            C->parent->right = M;
        }
    }
    if (M != NULL)
    {
        M->parent = C->parent;
    }
}

int deleteFromRBTree(RBTree *tree, void *data)
{
    if (!RBTreeContains(tree, data))
    {
        return false;
    }

    Node *M = getNode(tree, data);
    if (M->left != NULL && M->right != NULL)
    {
        Node *pred = precessor(M);
        M->data = pred->data;
        M = pred;
    }
    Node *C = getSon(M);
    if (M->color == BLACK)
    {
        if (C != NULL)
        {
            M->color = C->color;
        }
        else
        {
            M->color = BLACK;
        }

        fixDelete(M);
    }
    swap(tree, M, C);
    free(M);
    //freeNode(tree, M);

}