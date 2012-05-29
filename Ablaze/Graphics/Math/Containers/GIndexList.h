//
//  GIndexList.h
//  GParticles
//
//  Created by Caleb Jordan on 8/20/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#pragma once

#include <memory>

#pragma GCC visibility push(default)

class gIndexList {
private:
    struct gNode {
        struct gNode *next, *last;
        unsigned int index;
    };
    
    gNode *head, *tail;
    unsigned int count;
    
    int eoRecTree, eoSubTree;
    
public:
    class iterator {
        friend class gIndexList;
        gNode *curr;
    public:
        inline unsigned int value() { return curr->index; }
        inline bool next() { return curr = curr->next; }
        inline bool last() { return curr = curr->last; }
        inline bool good() { return curr; }
        inline bool operator==(iterator other) { return curr == other.curr; }
        bool operator+=(int step) {
            for (int i = 0; i < step; i++) {
                if (curr) curr = curr->next;
                else return false;
            }
            return curr;
        }
    };
    
public:
    gIndexList();
    gIndexList(unsigned int startIndex, unsigned int endIndex);
    ~gIndexList();
    
    iterator front();
    iterator back();
    iterator end();
    
    unsigned int size() { return count; }
    
    void insert(iterator &it, unsigned int index);
    void insertRange(iterator &it, unsigned int start, unsigned int end);
    void remove(iterator &it, unsigned int length = 1);
    void removeRange(unsigned int start, unsigned int end);
    
    void pushFront(unsigned int index);
    void pushBack(unsigned int index);
    unsigned int popFront();
    unsigned int popBack();
    
    void dissolve(iterator &it, gIndexList &other);
    gIndexList *extractNew(iterator &it, unsigned int length);
    
    void sortOddEvenMerge(void *thunkData, int (*comp)(void *, unsigned int, unsigned int));
    void sortStepOddEvenMerge(void *thunkData, int (*comp)(void *, unsigned int, unsigned int), int steps = 1);
    void sortOddEvenMerge(void *thunkData, int (*comp)(void *, unsigned int, unsigned int), int recTree, int subTree);
    
protected:
    void orphanNodes();
    
private:
    void init();
    void createFirst();
    void deleteFirst();
};

#pragma GCC visibility pop