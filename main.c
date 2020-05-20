//
// Created by odedw on 12-Oct-19.
//

#ifndef TA_EX3_PRODUCTEXAMPLE_C
#define TA_EX3_PRODUCTEXAMPLE_C

#include "utilities/RBUtilities.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LESS (-1)
#define EQUAL (0)
#define GREATER (1)

/**
 * Comparator for ProductExample
 * @param a ProductExample*
 * @param b ProductExample*
 * @return -1 if a<b, 0 if a==b, 1 if b<a
 */
int productComparatorByName(const void *a, const void *b)
{
    int *first = (int *) a;
    int *second = (int *) b;
    if (*first - *second < 0)
    {
        return LESS;
    }
    else if (*first - *second > 0)
    {
        return GREATER;
    }
    else
    {
        return EQUAL;
    }
}

void productFree(void *a)
{
    int *pProduct = (int *) a;
    free(pProduct);
    free(a);
}

/**
 *
 * @param pProduct pointer to product to print
 * @param null required argument for typedef
 * @return
 */
char  *printProduct(void *pProduct)
{
    char *str = (char *)malloc((10));
    int *product = (int *) pProduct;
    sprintf(str, "%d", *product);

    return str;
}

/**
 *
 * @return products for tests
 */
int **getProducts()
{

    int **products = (int **) malloc(sizeof(int *) * 6);

    products[0] = (int *) malloc(sizeof(int));
    products[1] = (int *) malloc(sizeof(int));
    products[2] = (int *) malloc(sizeof(int));
    products[3] = (int *) malloc(sizeof(int));
    products[4] = (int *) malloc(sizeof(int));
    products[5] = (int *) malloc(sizeof(int));

    *products[0]= 8;
    *products[1] = 2;
    *products[2] = 4;
    *products[3] = 10;
    *products[4] = 25;
    *products[5] = 7;

    return products;

}
/**
void freeResources(RBTree **tree, ProductExample ***products)
{
    freeRBTree(tree);
    productFree((*products)[1]);
    productFree((*products)[5]);
    free(*products);
}
**/
void assertion(int passed, int assertion_num, char *msg)
{
    if (!passed)
    {
        printf("assertion %d failed: %s\n", assertion_num, msg);
    }

}

int main()
{
    int **products = getProducts();
    RBTree *tree = newRBTree(productComparatorByName, productFree);
    insertToRBTree(tree, products[2]);
    insertToRBTree(tree, products[1]);
    insertToRBTree(tree, products[3]);
    insertToRBTree(tree, products[5]);
    insertToRBTree(tree, products[0]);
    //insertToRBTree(tree, products[1]);
    deleteFromRBTree(tree, products[2]);
    deleteFromRBTree(tree, products[3]);
    deleteFromRBTree(tree, products[1]);
    printRBTree(tree->root);


    /**int i = 0;
    for (i = 0; i < 6; i++)
    {
        if (RBTreeContains(tree, products[i]))
        {
            printf("\"%s\" is in the tree.\n", products[i]->name);
            if (i == 1 || i == 5)
            {
                printf(" This product should not be in the tree!\nTest failed, aborting");
                freeResources(&tree, &products);
                return 1;
            }
        }
        else
        {
            printf("\"%s\" is not in the tree.\n", products[i]->name);
            if (i != 1 && i != 5)
            {
                printf(" This product should be in the tree!\nTest failed, aborting");
                freeResources(&tree, &products);
                return 2;
            }
        }
    }

    printf("\nThe number of products in the tree is %lu.\n\n", tree->size);
    forEachRBTree(tree, printProduct, NULL);
    freeResources(&tree, &products);
    printf("test passed\n");
    return 0;**/
}


#endif //TA_EX3_PRODUCTEXAMPLE_C
