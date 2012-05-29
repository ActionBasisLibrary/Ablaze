//
//  GKeyframeValue.h
//  GMath
//
//  Created by Caleb Jordan on 8/30/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#pragma once

#include "GBinaryTree.h"
#include "GEasing.h"

#pragma GCC visibility push(default)

class gKeyframeValue {
public:
    struct gKeyframe {
        double time;
        double value;
        gEasing::gEasingFunction curve;
        
        gKeyframe()
        : time(0.0), value(0.0), curve(gEasing::gLinear) {}
        gKeyframe(double t, double val, gEasing::gEasingFunction func = gEasing::gLinear)
        : time(t), value(val), curve(func) {}
    };
private:
    double time;
    bool dirty;
    
    gBinaryTree keyframeTree;
    gKeyframe *lastKey, *nextKey;
    
public:
    gKeyframeValue();
    
    void insertKeyframe(gKeyframe &key);
    bool getNextKeyframe(double t, gKeyframe *key);
    bool deleteNextKeyframe(double t);
    bool getLastKeyframe(double t, gKeyframe *key);
    bool deleteLastKeyframe(double t);
    
    void rebalanceTree();
    
    void advance(double dt);
    void setTime(double t);
    inline double getTime() { return time; }
    
    double getValue();
    double getValueAt(double t);
    
private:
    void init();
    
    void findKeyframes();
};

#pragma GCC visibility pop