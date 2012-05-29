//
//  GKeyframeValue.cpp
//  GMath
//
//  Created by Caleb Jordan on 8/30/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GKeyframeValue.h"

static int gKeyframeCompare(const void *one, const void *two)
{
    gKeyframeValue::gKeyframe *keyOne = (gKeyframeValue::gKeyframe *)one;
    gKeyframeValue::gKeyframe *keyTwo = (gKeyframeValue::gKeyframe *)two;
    
    if (keyTwo->time == keyOne->time) return 0;
    else if (keyTwo->time > keyOne->time) return 1;
    else return -1;
}

gKeyframeValue::gKeyframeValue()
: keyframeTree(gKeyframeCompare)
{
    init();
}

#pragma mark DATA

void gKeyframeValue::insertKeyframe(gKeyframe &key)
{
    gKeyframe *newKey = new gKeyframe(key.time, key.value, key.curve);
    
    keyframeTree.insertValue(newKey);
    
    dirty = true;
}

bool gKeyframeValue::getNextKeyframe(double t, gKeyframe *key)
{
    gKeyframe testKey(t, 0.0);
    void *value = keyframeTree.getGreater(&testKey);
    
    if (value) {
        gKeyframe *nextKey = (gKeyframe *)value;
        *key = *nextKey;
    }
    
    return value != NULL;
}

bool gKeyframeValue::deleteNextKeyframe(double t)
{
    gKeyframe key(t, 0.0);
    void *val = keyframeTree.getGreater(&key);
    
    if (val) {
        keyframeTree.removeValue(val);
        dirty = true;
    }
    
    return val != NULL;
}

bool gKeyframeValue::getLastKeyframe(double t, gKeyframe *key)
{
    gKeyframe testKey(t, 0.0);
    void *value = keyframeTree.getLesserOrEqual(&testKey);
    
    if (value) {
        gKeyframe *nextKey = (gKeyframe *)value;
        *key = *nextKey;
    }
    
    return value != NULL;
}

bool gKeyframeValue::deleteLastKeyframe(double t)
{
    gKeyframe key(t, 0.0);
    void *val = keyframeTree.getLesserOrEqual(&key);
    
    if (val) {
        keyframeTree.removeValue(val);
        dirty = true;
    }
    
    return val != NULL;
}

void gKeyframeValue::rebalanceTree()
{
    keyframeTree.rebalance();
}

#pragma mark ACTIONS

void gKeyframeValue::advance(double dt)
{
    time += dt;
    
    if (nextKey && !dirty && time >= nextKey->time) {
        lastKey = nextKey;
        nextKey = (gKeyframe *)keyframeTree.getGreater(lastKey);
    }
}

void gKeyframeValue::setTime(double t)
{
    time = t;
    dirty = true;
}

double gKeyframeValue::getValue()
{
    if (dirty) findKeyframes();
    
    if (lastKey && nextKey) {
        double ntime = (time - lastKey->time) / (nextKey->time - lastKey->time);
        double range = nextKey->value - lastKey->value;
        
        return lastKey->value + range * nextKey->curve(ntime);
    } else if (lastKey) {
        return lastKey->value;
    } else if (nextKey) {
        return nextKey->value;
    } else {
        return 0.0;
    }
}

double gKeyframeValue::getValueAt(double t)
{
    gKeyframe *last, *next, test(t, 0.0);
    last = (gKeyframe *)keyframeTree.getLesserOrEqual(&test);
    next = (gKeyframe *)keyframeTree.getGreater(&test);
    
    if (lastKey && nextKey) {
        double ntime = (time - last->time) / (next->time - last->time);
        double range = next->value - last->value;
        
        return last->value + range * next->curve(ntime);
    } else if (last) {
        return last->value;
    } else if (next) {
        return next->value;
    } else {
        return 0.0;
    }
}

void gKeyframeValue::init()
{
    time = 0.0;
    lastKey = nextKey = NULL;
    dirty = true;
}

void gKeyframeValue::findKeyframes()
{
    gKeyframe key(time, 0.0);
    lastKey = (gKeyframe *)keyframeTree.getValue(&key);
    if (!lastKey) lastKey = (gKeyframe *)keyframeTree.getLesserOrEqual(&key);
    nextKey = (gKeyframe *)keyframeTree.getGreater(&key);
    dirty = false;
}