#pragma once

#include <stdio.h>

typedef struct AiroportsData {
    char* code;
    char* name;
} AiroportsData;

typedef struct Node {
    int height;
    AiroportsData* airoportsData;
    struct Node* leftChild;
    struct Node* rightChild;
} Node;

typedef struct AVLTree {
    Node* root;
} AVLTree;

int getHeightNode(Node* node);
int getBalanceNode(Node* node);
void updateHeightNode(Node* node);

AVLTree* mallocAVLTree();

Node* applyRotationNode(Node* node);
Node* rotateLeft(Node* node);
Node* rotateRight(Node* node);

Node* getMinNode(Node* node);

void insertToAVLTree(AVLTree* AVLTree, AiroportsData* airoportsData);
Node* insertNode(Node* node, AiroportsData* airoportsData);

AiroportsData* findByCodeAVLTree(AVLTree* AVLTree, char* code);

void deleteByCodeAVLTree(AVLTree* AVLTree, char* code);
Node* deleteNode(Node* node, char* code, int isDeleted);

void freeAVLTree(AVLTree** AVLTree);
void freeNode(Node* node);

void printAVLTree(AVLTree* AVLTree);
void printNode(Node* Node);

AiroportsData* toAiroportsData(char* nextLine);

int saveDataToFile(AVLTree* AVLTree, FILE* file);
int saveNode(Node* node, FILE* file);
