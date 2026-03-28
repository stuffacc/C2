#include "avl_tree.h"

#include <stdlib.h>
#include <string.h>

AVLTree* mallocAVLTree()
{
    AVLTree* AVLTree = malloc(sizeof(AVLTree));

    AVLTree->root = NULL;

    return AVLTree;
}

int getHeightNode(Node* node)
{
    if (node == NULL) {
        return 0;
    }

    return node->height;
}

int getBalanceNode(Node* node)
{
    if (node == NULL) {
        return 0;
    }

    int heightLeft = getHeightNode(node->leftChild);
    int heightRight = getHeightNode(node->rightChild);

    return heightLeft - heightRight;
}

void updateHeightNode(Node* node)
{
    if (node == NULL) {
        return;
    }

    int heightLeft = getHeightNode(node->leftChild);
    int heightRight = getHeightNode(node->rightChild);

    node->height = 1 + ((heightLeft > heightRight) ? heightLeft : heightRight);
}

Node* applyRotationNode(Node* node)
{
    int balance = getBalanceNode(node);

    // left heavy
    if (balance >= 2) {
        if (getBalanceNode(node->leftChild) <= -1) {
            node->leftChild = rotateLeft(node->leftChild);
        }

        return rotateRight(node);
    }

    // right heavy
    if (balance <= -2) {
        if (getBalanceNode(node->rightChild) >= 1) {
            node->rightChild = rotateRight(node->rightChild);
        }

        return rotateLeft(node);
    }

    return node;
}

Node* rotateLeft(Node* node)
{
    Node* rightNode = node->rightChild;

    node->rightChild = rightNode->leftChild;
    rightNode->leftChild = node;

    updateHeightNode(node);
    updateHeightNode(rightNode);

    return rightNode;
}

Node* rotateRight(Node* node)
{
    Node* leftNode = node->leftChild;

    node->leftChild = leftNode->rightChild;
    leftNode->rightChild = node;

    updateHeightNode(node);
    updateHeightNode(leftNode);

    return leftNode;
}

void insertToAVLTree(AVLTree* AVLTree, AiroportsData* airoportsData)
{
    if (AVLTree == NULL || airoportsData == NULL || airoportsData->code == NULL || airoportsData->name == NULL) {
        return;
    }

    AVLTree->root = insertNode(AVLTree->root, airoportsData);
}

Node* insertNode(Node* node, AiroportsData* airoportsData)
{
    if (node == NULL) {
        Node* newNode = malloc(sizeof(Node));
        newNode->height = 1;

        newNode->leftChild = NULL;
        newNode->rightChild = NULL;

        newNode->airoportsData = airoportsData;

        return newNode;
    }

    int cmp = strcmp(airoportsData->code, node->airoportsData->code);
    if (cmp < 0) {
        node->leftChild = insertNode(node->leftChild, airoportsData);
    }

    else if (cmp > 0) {
        node->rightChild = insertNode(node->rightChild, airoportsData);
    }

    else {
        free(airoportsData);
        return node;
    }

    updateHeightNode(node);

    return applyRotationNode(node);
}

AiroportsData* findByCodeAVLTree(AVLTree* AVLTree, char* code)
{
    if (AVLTree == NULL || code == NULL) {
        return NULL;
    }

    Node* node = AVLTree->root;
    while (node != NULL) {
        int cmp = strcmp(code, node->airoportsData->code);
        if (cmp < 0) {
            node = node->leftChild;
        }

        else if (cmp > 0) {
            node = node->rightChild;
        }

        else {
            return node->airoportsData;
        }
    }

    return NULL;
}

void deleteByCodeAVLTree(AVLTree* AVLTree, char* code)
{
    if (AVLTree == NULL || code == NULL) {
        return;
    }

    AVLTree->root = deleteNode(AVLTree->root, code, 0);
}

Node* getMinNode(Node* node)
{
    if (node->leftChild == NULL) {
        return node;
    }

    return getMinNode(node->leftChild);
}

Node* deleteNode(Node* node, char* code, int isDeleted)
{
    if (node == NULL) {
        return NULL;
    }

    int cmp = strcmp(code, node->airoportsData->code);

    if (cmp < 0) {
        node->leftChild = deleteNode(node->leftChild, code, isDeleted);
    }

    else if (cmp > 0) {
        node->rightChild = deleteNode(node->rightChild, code, isDeleted);
    }

    else {
        int nodeHasLeftChild = (node->leftChild != NULL);
        int nodeHasRightChild = (node->rightChild != NULL);

        if (nodeHasLeftChild && nodeHasRightChild) {
            Node* tempNode = getMinNode(node->rightChild);

            free(node->airoportsData->code);
            free(node->airoportsData->name);
            free(node->airoportsData);

            node->airoportsData = tempNode->airoportsData;

            node->rightChild = deleteNode(node->rightChild, tempNode->airoportsData->code, 1);
        }

        else {
            if (!isDeleted) {
                free(node->airoportsData->code);
                free(node->airoportsData->name);
                free(node->airoportsData);
            }

            free(node);

            if (nodeHasLeftChild) {
                Node* nodeTemp = node->leftChild;

                return nodeTemp;
            }

            else if (nodeHasRightChild) {
                Node* nodeTemp = node->rightChild;

                return nodeTemp;
            }

            else {
                return NULL;
            }
        }
    }

    updateHeightNode(node);

    return applyRotationNode(node);
}

void printAVLTree(AVLTree* AVLTree)
{
    printNode(AVLTree->root);
    printf("\n");
}

void printNode(Node* node)
{
    if (node == NULL) {
        return;
    }

    printNode(node->leftChild);
    printf("%s: %s, height: %d\n", node->airoportsData->code, node->airoportsData->name, node->height);
    printNode(node->rightChild);
}

void freeAVLTree(AVLTree** AVLTree)
{
    if (*AVLTree == NULL) {
        return;
    }

    freeNode((*AVLTree)->root);

    free(*AVLTree);
    *AVLTree = NULL;
}

void freeNode(Node* node)
{
    if (node == NULL) {
        return;
    }

    freeNode(node->leftChild);
    freeNode(node->rightChild);

    free(node->airoportsData->code);
    free(node->airoportsData->name);
    free(node->airoportsData);

    free(node);
}

AiroportsData* toAiroportsData(char* nextLine)
{
    int isInFormat = 0;
    char* startLine = nextLine;
    while (*nextLine != '\0') {
        if (*nextLine == ':') {
            isInFormat = 1;
            break;
        }

        nextLine++;
    }

    if (!isInFormat) {
        return NULL;
    }

    char* code = malloc(sizeof(char) * (nextLine - startLine + 1));

    *nextLine = '\0';

    code = strcpy(code, startLine);

    *nextLine = ':';

    nextLine++;
    startLine = nextLine;

    while (*nextLine != '\0') {
        nextLine++;
    }

    char* name = malloc(sizeof(char) * (nextLine - startLine + 1));
    name = strcpy(name, startLine);

    AiroportsData* airoportsData = malloc(sizeof(AiroportsData));
    airoportsData->code = code;
    airoportsData->name = name;

    return airoportsData;
}

int saveDataToFile(AVLTree* AVLTree, FILE* file)
{
    return saveNode(AVLTree->root, file);
}

int saveNode(Node* node, FILE* file)
{
    if (node == NULL) {
        return 0;
    }

    int leftSave = saveNode(node->leftChild, file);
    fprintf(file, "%s:%s\n", node->airoportsData->code, node->airoportsData->name);

    int rightSave = saveNode(node->rightChild, file);

    return leftSave + rightSave + 1;
}