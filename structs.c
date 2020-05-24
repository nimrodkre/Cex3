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

/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b)
{
    char *a1 = (char *)a;
    char *b1 = (char *)b;
    return strcmp(a1, b1);
}

/**
 * ForEach function that concatenates the given word and \n to pConcatenated. pConcatenated is
 * already allocated with enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, other on success
 */
int concatenate(const void *word, void *pConcatenated)
{
    if (word == NULL || pConcatenated == NULL)
    {
        return false;
    }
    char *concated = (char *)pConcatenated;
    char *newWord = (char *)word;
    strcat(concated, newWord);
    strcat(concated, "\n");
    return true;
}

/**
 * FreeFunc for strings
 */
void freeString(void *s)
{
    char *s1 = (char *)s;
    free(s1);
}

/**
 * finds the minimum between the 2
 * @param a the first number
 * @param b the second number
 * @return the minimum number between the two
 */
int min(int a, int b)
{
    if (a < b)
    {
        return a;
    }
    return b;
}

/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
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
    if (vec1->len > vec2->len)
    {
        return BIGGER;
    }
    return SAME;
}

/**
 * FreeFunc for vectors
 */
void freeVector(void *pVector)
{
    Vector *vec = (Vector *)pVector;
    double *vec_data = vec->vector;
    free(vec_data);
    free(vec);
    pVector = NULL;
}

/**
 * returns the normal squared of the vector
 * @param vec the vector to get its norm squared
 * @return the normal
 */
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

/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector->vector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector
 * @return 1 on success, 0 on failure (if pVector == NULL: failure).
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
    if (pVector == NULL || pMaxVector == NULL)
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

/**
 * traverses the tree and checks each node if its the larges
 * @param root the node to check
 * @param maxVec the current maximum vector
 */
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

/**
 * @param tree a pointer to a tree of Vectors
 * @return pointer to a *copy* of the vector that has the largest norm (L2 Norm).
 */
Vector *findMaxNormVectorInTree(RBTree *tree)
{
    if (tree == NULL)
    {
        return NULL;
    }
    Vector *maxVec = (Vector *)(tree->root->data);
    Vector *copyVec = (Vector *)malloc(sizeof(Vector));
    findMaxNormVecHelper(tree->root, maxVec);
    memcpy(copyVec, maxVec, sizeof(Vector));
    return maxVec;
}