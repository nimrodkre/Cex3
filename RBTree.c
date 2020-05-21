//
// Created by User on 19/05/2020.
//
#include "RBTree.h"
#include <stdlib.h>
#include "stdbool.h"

void fixInsert(RBTree *tree, Node* node);
void fixBothBlack(RBTree *tree, Node *node);

/**
 * Builds a new RedBlack Tree object
 * @param compFunc The compare function
 * @param freeFunc the free function
 * @return The new RBTreeFunction - user must call free
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

/**
 * Checks that the given tree contains the data received
 * @param tree the RBTree
 * @param data the data to check if in the tree
 * @return 1 if in else 0
 */
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

/**
 * Creates a node with the given data
 * @param data the data of the node
 * @return the new node, in HEAP!!!!
 */
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

/**
 * changes the places of both of the nodes in the tee
 * @param pivot the first node
 * @param newParent the new parent of the pivot
 */
void ChangePlaces(Node *pivot, Node *newParent)
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

/**
 * Rotate the tree to the left
 * @param tree our tree
 * @param pivot pivot spot of the rotation
 */
void rotateLeft(RBTree *tree, Node *pivot)
{
    Node *son = pivot->right;
    if (pivot == tree->root)
    {
        tree->root = son;
    }

    ChangePlaces(pivot, son);
    pivot->right = son->left;
    if (son->left != NULL)
    {
        son->left->parent = pivot;
    }
    son->left = pivot;
}

/**
 * rotation to the right
 * @param tree the RBTree
 * @param pivot the pivot on which to rotate the tree to the right
 */
void rotateRight(RBTree *tree, Node *pivot)
{
    Node *son = pivot->left;

    if (pivot == tree->root)
    {
        tree->root = son;
    }

    ChangePlaces(pivot, son);
    pivot->left = son->right;
    if (son->right != NULL)
    {
        son->right->parent = pivot;
    }
    son->right = pivot;
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

/**
 * getst the uncle of the given node
 * @param son the node of which to get its uncle
 * @return uncle
 */
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

/**
 * Insert case for when the dad is red and uncle is red
 * @param tree the tree on which we will insert
 * @param node the node to enter
 */
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

/**
 * AFter case 4 the node isn't in the correct location and we must fix it
 * @param tree the tree to insert to
 * @param node our node to insert
 */
void insertCase4_2(RBTree *tree, Node *node)
{
    Node *father = node->parent;
    Node *grandpa = father->parent;

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

/**
 * Parent is red and the uncle is black
 * @param tree the tree to change
 * @param node the node to insert
 */
void insertCase4(RBTree *tree, Node *node)
{
    Node *father = node->parent;
    Node *grandpa = father->parent;
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
    insertCase4_2(tree, node);
}

/**
 * Fixes the tree after inserting according to the casses written
 * @param tree the tree which we entered into
 * @param node the node which we inserted
 */
void fixInsert(RBTree *tree, Node* node)
{
    Node *father = node->parent;
    // CASE1: We inserted a root
    if (father == NULL)
    {
        node->color = BLACK;
        tree->root = node;
        return;
    }
    // CASE 2: BLACK
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
    // CASE 4: father is red and uncle is black
    insertCase4(tree, node);

}

/**
 * Algorithm which enters the given data to the tree, and also
 * builds it our own node
 * @param tree the tree to insert to
 * @param data the data to insert
 * @return true if worked else false
 */
int insertToRBTree(RBTree *tree, void *data)
{
    if (tree == NULL || data == NULL)
    {
        return false;
    }
    
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

/**
 * swaps the values of the 2 nodes
 * @param node1 the first node
 * @param node2 the second node
 */
void swapValues(Node *node1, Node *node2)
{
    void *temp = (void *)node1->data;
    node1->data = node2->data;
    node2->data = temp;
}

/**
 * gets the brother of the given node
 * @param node the node of which to get its brother
 * @return the brother
 */
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

/**
 * finds the successor of the given node
 * @param node the node of which to get its successor
 * @return the successor
 */
Node *successor( Node *node)
{
    Node *succ = node->right;
    while (succ->left != NULL)
    {
        succ = succ->left;
    }
    return succ;
}

/**
 * Corresponding to teh BST replace before we delete a node
 * @param node the node to delete
 * @return The node which we will be replacing with
 */
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

/**
 * in delete function when the brother is red, must fix up
 * @param tree the tree
 * @param node the node which we have a problem with its brother
 */
void brotherRedCase(RBTree *tree, Node *node)
{
    Node *brother = getBrother(node);
    Node *father = node->parent;
    father->color = RED;
    brother->color = BLACK;
    if (brother == brother->parent->left)
    {
        rotateRight(tree, father);
    }
    else
    {
        rotateLeft(tree, father);
    }
    fixBothBlack(tree, node);
}

/**
 * in delete function when the son of the brother is red, must fix up
 * @param tree the tree
 * @param node the node which we have a problem with its brother
 */
void brotherLeftSonRedCase(RBTree *tree, Node *node)
{
    Node *brother = getBrother(node);
    Node *father = node->parent;
    if (brother == brother->parent->left)
    {
        brother->left->color = brother->color;
        brother->color = father->color;
        rotateRight(tree, father);
    }
    else
    {
        brother->left->color = father->color;
        rotateRight(tree, brother);
        rotateLeft(tree, father);
    }
}

/**
 * in delete function when the right son of the brother is red, must fix up
 * @param tree the tree
 * @param node the node which we have a problem with its brother
 */
void brotherRightSonRedCase(RBTree *tree, Node *node)
{
    Node *brother = getBrother(node);
    Node *father = node->parent;
    if (brother == brother->parent->left)
    {
        brother->right->color = father->color;
        rotateLeft(tree, brother);
        rotateRight(tree, father);
    }
    else
    {
        brother->right->color = brother->color;
        brother->color = father->color;
        rotateLeft(tree, father);
    }
}

/**
 * In delete as stated in the targil if we have 2 blacks in a row, we will
 * have a problem when deleting. DB problem in other words
 * @param tree the tree
 * @param node the node which we want to take care of
 */
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
            brotherRedCase(tree, node);
        }
        else
        {
            if ((brother->left != NULL && brother->left->color == RED) ||
                (brother->right != NULL && brother->right->color == RED))
            {
                if (brother->left != NULL && brother->left->color == RED)
                {
                    brotherLeftSonRedCase(tree, node);
                }
                else
                {
                    brotherRightSonRedCase(tree, node);
                }
                father->color = BLACK;
            }
            else
            {
                // brothers sons aren't RED!!!!!
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

/**
 * in charge of freeing the given node
 * @param tree the tree
 * @param node the node to free
 */
void freeNode(Node *node)
{
    free(node);
}

/**
 * if we are similiar to bst leaf than we have a bit more work
 * @param tree the tree from which to delete
 * @param node  the node to delete
 * @param bothBlack problem 3 from targil
 */
void BSTLikeLeaf(RBTree *tree, Node *node, int bothBlack)
{
    Node *replace = BSTLikeReplace(node);
    Node *father = node->parent;
    if (node == tree->root)
    {
        node->data = replace->data;
        node->left = NULL;
        node->right = NULL;
        freeNode(replace);
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
        freeNode(node);
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
}

/**
 * when deleting the case in which the given node has no successor to switch with
 * @param tree the tree from which we will delete
 * @param node the node to delete
 * @param bothBlack - if node is black than we will have a bit more work
 */
void noSuccessor(RBTree *tree, Node *node, int bothBlack)
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
    freeNode(node);
}

/**
 * function in charge of deleting a node in the tree
 * @param tree the tree from which we want to delete
 * @param node the node to delete
 */
void deleteNode(RBTree *tree, Node *node)
{
    // the node which we would replace with if it was a BST
    Node *replace = BSTLikeReplace(node);
    int bothBlack = 0;
    //check problem 3 from the targil delete lesson
    if ((replace == NULL || replace->color == BLACK) && (node->color == BLACK))
    {
        bothBlack = 1;
    }

    if (replace == NULL)
    {
        noSuccessor(tree, node, bothBlack);
        return;
    }
    if (node->left == NULL || node->right == NULL)
    {
        BSTLikeLeaf(tree, node, bothBlack);
        return;
    }
    // well we have 2 songs, and have to do more work
    swapValues(node, replace);
    deleteNode(tree, replace);
}

/**
 * Finds the node and returns it with the given data
 * @param tree the tree to delete from
 * @param data the data to delete
 * @return the node which corresponds to the given data
 */
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

/**
 * deletes the given data from the tree
 * @param tree the tree to delete from
 * @param data the data to delete
 * @return 0 if not able to else 1
 */
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

/**
 * recursive function which does the given function on each node of the tree
 * @param root the current node to do the function on
 * @param func the function to do on each node
 * @param args the arguments to pass to function
 * @return 1 if able to else 0
 */
int forEachRBTreeHelper(const Node *root, forEachFunc func, void *args)
{
    if (root == NULL)
    {
        return true;
    }

    return forEachRBTreeHelper(root->left, func, args) && func(root->data, args) && forEachRBTreeHelper(root->right, func, args);
    /**if (forEachRBTreeHelper(root->left, func, args) == false)
    {
        return false;
    }
    if (func(root, args) == false)
    {
        return false;
    }
    if (forEachRBTreeHelper(root->right, func, args) == false)
    {
        return false;
    }
    return true;*/
}

/**
 * runs on all nodes and does the function
 * @param tree the tree
 * @param func function to use
 * @param args the arguments to pass to the function
 * @return 1 if good else 0
 */
int forEachRBTree(const RBTree *tree, forEachFunc func, void *args)
{
    if (tree == NULL || func == NULL)
    {
        return false;
    }
    return forEachRBTreeHelper(tree->root, func, args);
}

/**
 * frees all of the tree
 * @param tree the tree to work on
 * @param root the node to free
 */
void freeRBTreeHelper(RBTree *tree, Node *root)
{
    if (root == NULL)
    {
        return;
    }
    freeRBTreeHelper(tree, root->left);
    freeRBTreeHelper(tree, root->right);
    freeNode(root);
}

/**
 * frees all of the tree
 * @param tree the tree to free
 */
void freeRBTree(RBTree **tree)
{
    if (tree == NULL)
    {
        return;
    }
    freeRBTreeHelper(*tree, (*tree)->root);
    free(*tree);
}