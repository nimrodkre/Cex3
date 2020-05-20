//
// Created by User on 19/05/2020.
//
#include <string.h>
#include <stdbool.h>
#include "Structs.h"
#include <stdlib.h>

#define SMALLER -1
#define BIGGER 1
#define SAME 0

int stringCompare(const void *a, const void *b)
{
    char *a1 = (char *)a;
    char *b1 = (char *)b;
    return strcmp(a, b);
}

int concatenate(const void *word, void *pConcatenated)
{
    if (word == NULL || pConcatenated == NULL)
    {
        return false;
    }
    char *concated = (char *)pConcatenated;
    char *newWord = (char *)word;
    strcat(concated, newWord);
    return true;
}

void freeString(void *s)
{
    char *s1 = (char *)s;
    free(s1);
}

int min(int a, int b)
{
    if (a < b)
    {
        return a;
    }
    return b;
}

int vectorCompare1By1(const void *a, const void *b)
{
    Vector *vec1 = (Vector *)a;
    Vector *vec2 = (Vector *)b;
    double *vec1Data = (double *)vec1->vector;
    double *vec2Data = (double *)vec2->vector;
    int len = min(vec1->len, vec2->len);
    int i = 0;
    for (i = 0; i < len; i++)
    {
        if (vec1Data[i] < vec2Data[i])
        {
            return SMALLER;
        }
        else if (vec1Data[i] > vec2Data[i])
        {
            return BIGGER;
        }
    }
    if (vec1->len < vec2->len)
    {
        return SMALLER;
    }
    else if (vec1->len > vec2->len)
    {
        return BIGGER;
    }
    return SAME;
}

void freeVector(void *pVector)
{
    Vector *vec = (Vector *)pVector;
    free(vec->vector);
}

double getNorm(const Vector *vec)
{
    double sum = 0;
    int i = 0;
    for (i = 0; i < vec->len; i++)
    {
        sum += vec->vector[i] * vec->vector[i];
    }
    return sum;
}

int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
    if (pVector == NULL)
    {
        return false;
    }
    Vector *p = (Vector *)pVector;
    double pNorm = getNorm(p);
    Vector *max = (Vector *)pMaxVector;
    double maxNorm = getNorm(max);
    if (pNorm > maxNorm)
    {
        memcpy(pMaxVector, pVector, sizeof(Vector));
    }
    return true;
}

void findMaxNormVecHelper(Node *root, Vector *maxVec)
{
    if (root == NULL)
    {
        return;
    }
    Vector *currVec = (Vector *)root->data;
    copyIfNormIsLarger(currVec, maxVec);
    findMaxNormVecHelper(root->left, maxVec);
    findMaxNormVecHelper(root->right, maxVec);
}

Vector *findMaxNormVectorInTree(RBTree *tree)
{
    if (tree == NULL)
    {
        return NULL;
    }
    Vector *maxVec = (Vector *)malloc(sizeof(Vector));
    maxVec = (Vector *)(tree->root->data);
    findMaxNormVecHelper(tree->root, maxVec);
    return maxVec;
}