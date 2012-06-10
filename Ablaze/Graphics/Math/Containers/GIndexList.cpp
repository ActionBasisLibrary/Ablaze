//
//  GIndexList.cpp
//  GParticles
//
//  Created by Caleb Jordan on 8/20/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GIndexList.h"

#define newNode ((gNode *)malloc(sizeof(gNode)))

#pragma mark CONSTRUCTORS

gIndexList::gIndexList()
{
    init();
}

gIndexList::gIndexList(unsigned int startIndex, unsigned int endIndex)
{
    init();
    iterator it = front();
    insertRange(it, startIndex, endIndex);
}

gIndexList::~gIndexList()
{
    if (!head) return;
    iterator it = front();
    
    do {
        gNode *curr = it.curr;
        it.next();
        free(curr);
    }
    while (it.good());
}

#pragma mark INSERTING AND REMOVING INDEXES

void gIndexList::insert(iterator &it, unsigned int index)
{
    gNode *aNode = newNode;
    
    aNode->index = index;
    aNode->last = it.curr ? it.curr->last : tail;
    aNode->next = it.curr;
    
    if (aNode->last) aNode->last->next = aNode;
    else head = aNode;
    if (aNode->next) aNode->next->last = aNode;
    else tail = aNode;
    count++;
}

void gIndexList::insertRange(iterator &it, unsigned int start, unsigned int end)
{
    if (start >= end) return;
    
    while (start < end) insert(it, start++);
}

void gIndexList::remove(iterator &it, unsigned int length)
{
    if (!it.curr || length == 0) return;
    
    iterator start = it;
    while (length != 0 && it.good()) {
        it.next();
        count--; length--;
    }
    
    // End the sequence to be removed if necessary
    if (it.curr) it.curr->last->next = NULL;
    
    // Patch up back of gap
    if (it.curr) it.curr->last = start.curr->last;
    else tail = start.curr->last;
    
    // Patch up front of gap
    if (start.curr->last) start.curr->last->next = it.curr;
    else head = it.curr;
    
    // Free everythin in the middle
    do {
        gNode *curr = start.curr;
        start.next();
        free(curr);
    } while (start.good());
}

void gIndexList::removeRange(unsigned int start, unsigned int end)
{
    iterator it = front();
    
    // If it is at valid point...
    while (it.good()) {
        iterator startIt = it;
        unsigned int length = 0;
        
        // Advance it while value fits, and up length of string
        while (it.good() && it.value() >= start && it.value() < end) {
            length++;
            it.next();
        }
        
        // Or else remove what we can and carry forth
        if (length > 0) remove(startIt, length);
        it.next();
    }
}

void gIndexList::pushFront(unsigned int index)
{
    gNode *aNode = newNode;
    count++;
    
    aNode->index = index;
    aNode->last = NULL;
    aNode->next = head;
    
    head = aNode;
    
    if (aNode->next) aNode->next->last = aNode;
    else tail = aNode;
}

void gIndexList::pushBack(unsigned int index)
{
    gNode *aNode = newNode;
    count++;
    
    aNode->index = index;
    aNode->last = tail;
    aNode->next = NULL;
    
    tail = aNode;
    
    if (aNode->last) aNode->last->next = aNode;
    else head = aNode;
}

unsigned int gIndexList::popFront()
{
    if (!head) return -1;
    
    unsigned int result = head->index;
    gNode *oldHead = head;
    
    head = head->next;
    
    if (head) head->last = NULL;
    else tail = NULL;
    
    free(oldHead);
    count--;
    return result;
}

unsigned int gIndexList::popBack()
{
    if (!tail) return -1;
    
    unsigned int result = tail->index;
    gNode *oldTail = tail;
    
    tail = tail->last;
    
    if (tail) tail->next = NULL;
    else head = NULL;
    
    free(oldTail);
    count--;
    return result;
}


#pragma mark INSERTING AND EXTRACTING LISTS

unsigned int *gIndexList::serialize()
{
    if (count == 0) return NULL;
    
    unsigned int *array = new unsigned int[count];
    unsigned int *ptr = array;
    
    gNode *curr = head;
    while (curr) {
        *ptr++ = curr->index;
        curr = curr->next;
    }
    
    return array;
}

unsigned short *gIndexList::serializeShort()
{
    if (count == 0) return NULL;
    
    unsigned short *array = new unsigned short[count];
    unsigned short *ptr = array;
    
    gNode *curr = head;
    while (curr) {
        *ptr++ = (unsigned short)curr->index;
        curr = curr->next;
    }
    
    return array;
}

void gIndexList::dissolve(iterator &it, gIndexList &other)
{
    if (other.size() < 1) return;
    count += other.size();
    
    gNode *last = it.curr ? it.curr->last : tail;
    gNode *next = it.curr;
    
    iterator start = other.front();
    iterator end = other.back();
    
    start.curr->last = last;
    start.curr->next = next;
    
    if (last) last->next = start.curr;
    else head = start.curr;
    
    if (next) next->last = end.curr;
    else tail = end.curr;
    
    other.orphanNodes();
}

gIndexList *gIndexList::extractNew(iterator &it, unsigned int length)
{
    if (length < 1 || !it.curr) return NULL;
    
    gIndexList *newList = new gIndexList;
    
    iterator start = it;
    
    while (it.next() && length-- > 0) {
        count--;
        newList->count++;
    }
    
    newList->head = start.curr;
    newList->tail = it.curr ? it.curr->last : tail;
    
    if (it.curr) it.curr->last = start.curr->last;
    else tail = start.curr->last;
    
    if (start.curr->last) start.curr->last->next = it.curr;
    else head = it.curr;
    
    return newList;
}

#pragma mark ITERATOR ACCESS

gIndexList::iterator gIndexList::front()
{
    iterator it;
    it.curr = head;
    return it;
}

gIndexList::iterator gIndexList::back()
{
    iterator it;
    it.curr = tail;
    return it;
}

gIndexList::iterator gIndexList::end()
{
    iterator it;
    it.curr = NULL;
    return it;
}

#pragma mark SORTING


void gIndexList::sortStepOddEvenMerge(void *thunkData, int (*comp)(void *, unsigned int, unsigned int), int steps)
{
    for (int i = 0; steps < 0 || i < steps; i++) {
        
        sortOddEvenMerge(thunkData, comp, eoRecTree, eoSubTree);

        eoSubTree /= 2;
        if (eoSubTree == 1) {
            eoRecTree *= 2;
            eoSubTree = eoRecTree;
            if (eoRecTree > count * 2) {
                eoSubTree = eoRecTree = 2;
            }
        }
    }
}

void gIndexList::sortOddEvenMerge(void *thunkData, int (*comp)(void *, unsigned int, unsigned int))
{
    int subTree = 2, recTree = 2;
    while (true) {
        sortOddEvenMerge(thunkData, comp, recTree, subTree);
        
        subTree /= 2;
        if (subTree == 1) {
            recTree *= 2;
            subTree = recTree;
            if (recTree > count * 2) return;
        }
    }
}

void gIndexList::sortOddEvenMerge(void *thunkData, int (*comp)(void *, unsigned int, unsigned int), int recTree, int subTree)
{
    if (count < 2) return;
    
    // Set up the dueling iterators!
    iterator upper = front(), downer = front();
    
    int compDiff = subTree / 2;
    
    if (recTree == subTree) {
        while (true) {
            // Assume upper == lower and both are start of each recTree chunk
            if (!(upper += compDiff)) return;
            
            for (int i = 0; i < compDiff; i++) {
                if (comp(thunkData, downer.value(), upper.value()) < 0) {
                    unsigned int temp = downer.curr->index;
                    downer.curr->index = upper.curr->index;
                    upper.curr->index = temp;
                }
                downer.next();
                if (!upper.next()) return;
            }
            
            downer = upper;
        }
    } else {
        while (true) {
            // Assume upper == lower and both are start of recTree chunk
            downer += compDiff;
            upper = downer;
            if (!(upper += compDiff)) return;
            
            // Sort center sections of subTree size
            for (int i = subTree; i < recTree; i += subTree) {
                // Assume its are in initial compare position, moved in compDiff from start
                for (int j = 0; j < compDiff; j++) {
                    if (comp(thunkData, downer.value(), upper.value()) < 0) {
                        unsigned int temp = downer.curr->index;
                        downer.curr->index = upper.curr->index;
                        upper.curr->index = temp;
                    }
                    downer.next();
                    if (!upper.next()) return;
                }
                downer = upper;
                if (!(upper += compDiff)) return;
            }
            
            downer = upper;
        }
    }
}

#pragma mark PROTECTED

void gIndexList::orphanNodes()
{
    init();
}

#pragma mark PRIVATE

void gIndexList::init()
{
    head = tail = NULL;
    count = 0;
    eoSubTree = eoRecTree = 2;
}

void gIndexList::createFirst()
{
    head = tail = newNode;
    head->last = head->next = NULL;
    count = 1;
}

void gIndexList::deleteFirst()
{
    free(head);
    head = tail = NULL;
    count = 0;
}

#undef newNode
