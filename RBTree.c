//
// Created by User on 19/05/2020.
//
#include "RBTree.h"
#include <stdlib.h>

RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
    RBTree *tree = (RBTree *)calloc(1, sizeof(RBTree));
    if (tree == NULL)
    {
        return NULL;
    }
    tree->freeFunc = freeFunc;
    tree->compFunc = compFunc;
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

