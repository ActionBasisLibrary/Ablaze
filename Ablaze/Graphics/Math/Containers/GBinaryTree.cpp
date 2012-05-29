//
//  GBinaryTree.cpp
//  GMath
//
//  Created by Caleb Jordan on 8/30/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GBinaryTree.h"

#include <vector>
#include <math.h>

#pragma mark CONSTRUCTORS AND STUFF

gBinaryTree::gBinaryTree(gCompare compareFunction)
{
    init();
    setCompare(compareFunction);
}

gBinaryTree::~gBinaryTree()
{
    releaseNode(rootNode);
}

#pragma mark CALLBACK SETTERS

void gBinaryTree::setCompare(gCompare functionPointer)
{
    compare = functionPointer;
}

void gBinaryTree::setRelease(gRelease functionPointer)
{
    release = functionPointer;
}

#pragma mark ACTIONS

// Not recursive--insertNode is a non-recursive function
void gBinaryTree::insertValue(void *value)
{
    gNode *iNode = new gNode();
    iNode->value = value;
    
    insertNode(&rootNode, iNode);
    
    size++;
}

// Not recursive
void *gBinaryTree::getValue(void *value)
{
    gNode *curr = rootNode;
    while (curr) {
        int diff = compare(curr->value, value);
        if (diff == 0) {
            return curr->value;
        } else if (diff < 0) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
    
    return NULL;
}

// Calls recursive
void *gBinaryTree::getGreater(void *value)
{
    return getGreater(&rootNode, value);
}

// Calls recursive
void *gBinaryTree::getLesser(void *value)
{
    return getLesser(&rootNode, value);
}

// Calls recursive
void *gBinaryTree::getGreaterOrEqual(void *value)
{
    return getGreaterOrEqual(&rootNode, value);
}

// Calls recursive
void *gBinaryTree::getLesserOrEqual(void *value)
{
    return getLesserOrEqual(&rootNode, value);
}

// Not recursive
bool gBinaryTree::removeValue(void *value)
{
    gNode **curr = &rootNode;
    while (*curr) {
        int diff = compare((*curr)->value, value);
        if (diff < 0) {
            curr = &(*curr)->left;
        } else if (diff > 0) {
            curr = &(*curr)->right;
        } else { // Sweet we found it
            gNode *marked = *curr;
            
            *curr = marked->right;
            insertNode(curr, marked->left);
            
            if (release) release(marked->value);
            delete marked;
            size--;
            
            return true;
        }
    }
    
    return false;
}

#pragma mark TRAVERSAL

void gBinaryTree::walkInOrder(gActor actor, void *extra)
{
    walkInOrder(rootNode, actor, extra);
}

void gBinaryTree::walkTopDown(gActor actor, void *extra)
{
    walkTopDown(rootNode, actor, extra);
}

void gBinaryTree::walkBottomUp(gActor actor, void *extra)
{
    walkBottomUp(rootNode, actor, extra);
}


size_t gBinaryTree::countSize()
{
    size = countSize(rootNode);
    return size;
}

size_t gBinaryTree::getSize()
{
    return size;
}

#pragma mark TRICKY SHIT

void gBinaryTree::rebalance()
{
    size_t count = 0;
    gNode tempRoot;
    tempRoot.right = rootNode;
    
    treeToVine(&tempRoot, &count);
    vineToTree(&tempRoot, count);
    
    rootNode = tempRoot.right;
}

#pragma mark PRIVATE

void gBinaryTree::init()
{
    rootNode = NULL;
    compare = NULL;
    release = NULL;
    
    size = 0;
}

void gBinaryTree::releaseNode(gNode *node)
{
    if (node) {
        releaseNode(node->left);
        releaseNode(node->right);
        
        if (release) release(node->value);
        
        delete node;
    }
}

void gBinaryTree::insertNode(gNode **node, gNode *iNode)
{
    if (!iNode) return;
    
    while (*node) {
        if (compare((*node)->value, iNode->value) <= 0) {
            node = &(*node)->left;
        } else {
            node = &(*node)->right;
        }
    }
    *node = iNode;
}

void *gBinaryTree::getGreater(gNode **node, void *value)
{
    if (!*node) {
        return NULL;
    } else {
        int diff = compare((*node)->value, value);
        if (diff >= 0) {
            return getGreater(&(*node)->right, value);
        } else {
            void *leftVal = getGreater(&(*node)->left, value);
            if (leftVal == NULL || compare(leftVal, value) >= 0) {
                return (*node)->value;
            } else {
                return leftVal;
            }
        }
    }
}

void *gBinaryTree::getGreaterOrEqual(gBinaryTree::gNode **node, void *value)
{
    if (!*node) {
        return NULL;
    } else {
        int diff = compare((*node)->value, value);
        if (diff > 0) {
            return getGreaterOrEqual(&(*node)->right, value);
        } else {
            void *leftVal = getGreaterOrEqual(&(*node)->left, value);
            if (leftVal == NULL || compare(leftVal, value) > 0) {
                return (*node)->value;
            } else {
                return leftVal;
            }
        }
    }
}

void *gBinaryTree::getLesser(gNode **node, void *value)
{
    if (!*node) {
        return NULL;
    } else {
        int diff = compare((*node)->value, value);
        if (diff <= 0) {
            return getLesser(&(*node)->left, value);
        } else {
            void *rightVal = getLesser(&(*node)->right, value);
            if (rightVal == NULL || compare(rightVal, value) <= 0) {
                return (*node)->value;
            } else {
                return rightVal;
            }
        }
    }
}

void *gBinaryTree::getLesserOrEqual(gBinaryTree::gNode **node, void *value)
{
    if (!*node) {
        return NULL;
    } else {
        int diff = compare((*node)->value, value);
        if (diff < 0) {
            return getLesserOrEqual(&(*node)->left, value);
        } else {
            void *rightVal = getLesserOrEqual(&(*node)->right, value);
            if (rightVal == NULL || compare(rightVal, value) < 0) {
                return (*node)->value;
            } else {
                return rightVal;
            }
        }
    }
}

#pragma mark RECURSIVE TRAVERSAL

void gBinaryTree::walkInOrder(gNode *node, gActor actor, void *extra)
{
    if (node) {
        walkInOrder(node->left, actor, extra);
        actor(node->value, extra);
        walkInOrder(node->right, actor, extra);
    }
}

void gBinaryTree::walkTopDown(gNode *node, gActor actor, void *extra)
{
    if (node) {
        actor(node->value, extra);
        walkTopDown(node->left, actor, extra);
        walkTopDown(node->right, actor, extra);
    }
}

void gBinaryTree::walkBottomUp(gNode *node, gActor actor, void *extra)
{
    if (node) {
        walkBottomUp(node->left, actor, extra);
        walkBottomUp(node->right, actor, extra);
        actor(node->value, extra);
    }
}

size_t gBinaryTree::countSize(gNode *node)
{
    if (node) {
        size_t left = countSize(node->left), right = countSize(node->right);
        return 1 + left + right;
    } else {
        return 0;
    }
}

#pragma mark TRICKY SHIT

void gBinaryTree::treeToVine(gNode *node, size_t *count)
{
    gNode *tail = node, *remainder = node->right, *temp;
    *count = 0;
    
    while (remainder) {
        if (!remainder->left) {
            tail = remainder;
            remainder = tail->right;
            (*count)++;
        } else {
            temp = remainder->left;
            remainder->left = temp->right;
            temp->right = remainder;
            
            remainder = temp;
            tail->right = temp;
        }
    }
}

void gBinaryTree::vineToTree(gNode *node, size_t count)
{
    int size = (int)count;
    int leafCount = count + 1 - pow(2, floor(log2(count + 1)));
    
    vineCompression(node, leafCount);
    size = size - leafCount;
    
    while (size > 1) {
        vineCompression(node, size / 2);
        size /= 2;
    }
}

void gBinaryTree::vineCompression(gNode *root, size_t count)
{
    gNode *curr, *child;
    curr = root;
    for (int i = 0; i < count; i++) {
        child = curr->right;
        curr->right = child->right;
        curr = curr->right;
        child->right = curr->left;
        curr->left = child;
    }
}