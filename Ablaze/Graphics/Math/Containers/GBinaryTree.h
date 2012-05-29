//
//  GBinaryTree.h
//  GMath
//
//  Created by Caleb Jordan on 8/30/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#pragma once

#include <memory.h>

#pragma GCC visibility push(default)

class gBinaryTree {
public:
    /*
     * This function will be used to sort--returns 0 if equal, <0 if one > two, >0 if one < two
     */
    typedef int (gCompare)(const void *one, const void *two);
    
    /*
     * This function will be used to release data--default won't do anything
     */
    typedef void (gRelease)(const void *value);
    
    /*
     * This function will do whatever
     */
    typedef void (gActor)(const void *value, void *extra);
    
private:
    struct gNode {
        gNode *left, *right;
        void *value;
        gNode() :left(NULL), right(NULL), value(NULL) {}
    };
    
    gNode *rootNode;
    gCompare *compare;
    gRelease *release;
    
    size_t size;
    
public:
    gBinaryTree(gCompare compare);
    ~gBinaryTree();
    
    void setCompare(gCompare functionPointer);
    void setRelease(gRelease functionPointer);
    
    void insertValue(void *value);
    bool removeValue(void *value);
    
    void *getValue(void *value);
    void *getGreater(void *value);
    void *getLesser(void *value);
    void *getGreaterOrEqual(void *value);
    void *getLesserOrEqual(void *value);
    
    void walkInOrder(gActor actor, void *extra);
    void walkTopDown(gActor actor, void *extra);
    void walkBottomUp(gActor actor, void *extra);
    
    size_t getSize();
    size_t countSize();
    
    void rebalance();
    
private:
    void init();
    
    void releaseNode(gNode *node);
    void insertNode(gNode **node, gNode *iNode);
    
    void *getGreater(gNode **node, void *value);
    void *getLesser(gNode **node, void *value);
    void *getGreaterOrEqual(gNode **node, void *value);
    void *getLesserOrEqual(gNode **node, void *value);
    
    void walkInOrder(gNode *node, gActor actor, void *extra);
    void walkTopDown(gNode *node, gActor actor, void *extra);
    void walkBottomUp(gNode *node, gActor actor, void *extra);
    size_t countSize(gNode *node);
    
    void treeToVine(gNode *node, size_t *count);
    void vineToTree(gNode *node, size_t count);
    void vineCompression(gNode *node, size_t count);
};

#pragma GCC visibility pop