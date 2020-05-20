//
// Created by User on 19/05/2020.
//
#include "RBTree.h"
#include <stdlib.h>
#include "stdbool.h"

void fixInsert(RBTree *tree, Node* node);

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
    if (tree == NULL || data == NULL)
    {
        return false;
    }
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

void rotationChangePlace(Node *pivot, Node *newParent)
{
    if (pivot->parent != NULL)
    {
        if (pivot == pivot->parent->left)
        {
            pivot->parent->left = newParent;
        }
        else
        {
            pivot->parent->right = newParent;
        }
    }
    newParent->parent = pivot->parent;
    pivot->parent = newParent;
}

void rotateLeft2(RBTree *tree, Node *pivot)
{
    Node *son = pivot->right;
    if (pivot == tree->root)
    {
        tree->root = son;
    }

    rotationChangePlace(pivot, son);
    pivot->right = son->left;
    if (son->left != NULL)
    {
        son->left->parent = pivot;
    }
    son->left = pivot;
}

void rotateRight2(RBTree *tree, Node *pivot)
{
    Node *son = pivot->left;

    if (pivot == tree->root)
    {
        tree->root = son;
    }

    rotationChangePlace(pivot, son);
    pivot->left = son->right;
    if (son->right != NULL)
    {
        son->right->parent = pivot;
    }
    son->right = pivot;
}

void rotateLeft(Node *pivot)
{
    Node *son = pivot->right;
    Node *parent = pivot->parent;
    if (son == NULL)
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
            parent->left = son;
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

void insertCase3(RBTree *tree, Node *node)
{
    Node *father = node->parent;
    Node *uncle = getUncle(node);
    Node *grandpa = father->parent;
    father->color = BLACK;
    uncle->color = BLACK;
    grandpa->color = RED;
    fixInsert(tree, grandpa);
}

void insertCase4_2(Node *node)
{
    Node *father = node->parent;
    Node *grandpa = father->parent;

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

void insertCase4(Node *node)
{
    Node *father = node->parent;
    Node *grandpa = father->parent;
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
    insertCase4_2(node);
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
    if (getUncle(node) != NULL && getUncle(node)->color == RED)
    {
        insertCase3(tree, node);
        return;
    }
    insertCase4(node);

}

int insertToRBTree(RBTree *tree, void *data)
{
    if (tree == NULL || data == NULL)
    {
        return false;
    }
    /**
    if (RBTreeContains(tree, data) == true)
    {
        return false;
    }*/
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
    void *temp = (void *)node1->data;
    node1->data = node2->data;
    node2->data = temp;
}

Node *getBrother(const Node *node)
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

Node *successor( Node *node)
{
    Node *succ = node->right;
    while (succ->left != NULL)
    {
        succ = succ->left;
    }
    return succ;
}

Node *BSTLikeReplace(Node *node)
{
    if (node->left != NULL && node->right != NULL)
    {
        return successor(node);
    }

    if (node->left == NULL && node->right == NULL)
    {
        return NULL;
    }

    if (node->left != NULL)
    {
        return node->left;
    }
    return node->right;
}

void fixBothBlack(RBTree *tree, Node *node)
{
    if (node == tree->root)
    {
        return;
    }

    Node *brother = getBrother(node);
    Node *father = node->parent;

    if (brother == NULL)
    {
        fixBothBlack(tree, father);
    }
    else
    {
        if (brother->color == RED)
        {
            father->color = RED;
            brother->color = BLACK;
            if (brother == brother->parent->left)
            {
                rotateRight2(tree, father);
            }
            else
            {
                rotateLeft2(tree, father);
            }
            fixBothBlack(tree, node);
        }
        else
        {
            if ((brother->left != NULL && brother->left->color == RED) ||
                (brother->right != NULL && brother->right->color == RED))
            {
                if (brother->left != NULL && brother->left->color == RED)
                {
                    if (brother == brother->parent->left)
                    {
                        brother->left->color = brother->color;
                        brother->color = father->color;
                        rotateRight2(tree, father);
                    }
                    else
                    {
                        brother->left->color = father->color;
                        rotateRight2(tree, brother);
                        rotateLeft2(tree, father);
                    }
                }
                else
                {
                    if (brother == brother->parent->left)
                    {
                        brother->right->color = father->color;
                        rotateLeft2(tree, brother);
                        rotateRight2(tree, father);
                    }
                    else
                    {
                        brother->right->color = brother->color;
                        brother->color = father->color;
                        rotateLeft2(tree, father);
                    }
                }
                father->color = BLACK;
            }
            else
            {
                brother->color = RED;
                if (father->color == BLACK)
                {
                    fixBothBlack(tree, father);
                }
                else
                {
                    father->color = BLACK;
                }
            }
        }
    }
}

void freeNode(RBTree *tree, Node *node)
{
    tree->freeFunc(node->data);
    free(node);
}

void deleteNode(RBTree *tree, Node *node)
{
    Node *replace = BSTLikeReplace(node);
    int bothBlack = 0;
    if ((replace == NULL || replace->color == BLACK) && (node->color == BLACK))
    {
        bothBlack = 1;
    }
    Node *father = node->parent;

    if (replace == NULL)
    {
        if (node == tree->root)
        {
            tree->root = NULL;
        }
        else
        {
            if (bothBlack == 1)
            {
                fixBothBlack(tree, node);
            }
            else
            {
                Node *brother = getBrother(node);
                if (brother != NULL)
                {
                    brother->color = RED;
                }
            }

            if (node == node->parent->left)
            {
                node->parent->left = NULL;
            }
            else
            {
                node->parent->right = NULL;
            }
        }
        freeNode(tree, node);
        return;
    }

    if (node->left == NULL || node->right == NULL)
    {
        if (node == tree->root)
        {
            node->data = replace->data;
            node->left = NULL;
            node->right = NULL;
            freeNode(tree, replace);
        }
        else
        {
            if(node == father->left)
            {
                father->left = replace;
            }
            else
            {
                father->right = replace;
            }
            freeNode(tree, node);
            replace->parent = father;
            if (bothBlack == 1)
            {
                fixBothBlack(tree, replace);
            }
            else
            {
                replace->color = BLACK;
            }
        }
        return;
    }
    swapValues(node, replace);
    deleteNode(tree, replace);
}

Node *getNode(RBTree *tree, const void *data)
{
    Node *root = tree->root;
    while (root != NULL)
    {
        if (root->data == data)
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
    return NULL;
}

int deleteFromRBTree(RBTree *tree, void *data)
{
    if (tree == NULL || data == NULL)
    {
        return false;
    }

    if (!RBTreeContains(tree, data))
    {
        return false;
    }
    Node *delNode = getNode(tree, data);
    deleteNode(tree, delNode);
    tree->size -= 1;
    return true;
}

int forEachRBTreeHelper(const Node *root, forEachFunc func, void *args)
{
    if (root->left != NULL)
    {
        if (forEachRBTreeHelper(root->left, func, args) == false)
        {
            return false;
        }
    }
    if (func(root, args) == false)
    {
        return false;
    }
    if (root->right != NULL)
    {
        if (forEachRBTreeHelper(root->right, func, args) == false)
        {
            return false;
        }
    }
    return true;
}

int forEachRBTree(const RBTree *tree, forEachFunc func, void *args)
{
    if (tree == NULL || func == NULL || args == NULL)
    {
        return false;
    }
    return forEachRBTreeHelper(tree->root, func, args);
}

void freeRBTreeHelper(RBTree *tree, Node *root)
{
    if (root == NULL)
    {
        return;
    }
    freeRBTreeHelper(tree, root->left);
    freeNode(tree, root);
    freeRBTreeHelper(tree, root->right);
}

void freeRBTree(RBTree **tree)
{
    if (tree == NULL)
    {
        return;
    }
    freeRBTreeHelper(*tree, (*tree)->root);
    free(*tree);
}