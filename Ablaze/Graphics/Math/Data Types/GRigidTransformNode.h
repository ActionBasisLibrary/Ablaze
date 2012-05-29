//
//  GRigidTransformNode.h
//  GMath
//
//  Created by Caleb Jordan on 9/1/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#pragma once

#include <vector>

#include "GRigidTransform.h"

#pragma GCC visibility push(default)

class gRigidTransformNode {
private:
    gRigidTransform localTransform, parentTransform, globalTransform;
    std::vector<gRigidTransformNode *> children;
    
    int numChildren;
    bool touched;
    
public:
    gRigidTransformNode();
    gRigidTransformNode(gRigidTransform &parentTransform);
    gRigidTransformNode(gRigidTransform &parentTransform, gRigidTransform &localTransform);
    
    gRigidTransform &getGlobalTransform();
    void setGlobalTransform(gRigidTransform &newGlobal, bool updateChildren = true);
    
    gRigidTransform &getParentTransform();
    void setParentTransform(gRigidTransform &newParent, bool updateChildren = true);
    
    gRigidTransform &getLocalTransform();
    void setLocalTransform(gRigidTransform &newLocal, bool updateChildren = true);
    
    bool addChild(gRigidTransformNode *child);
    bool removeChild(gRigidTransformNode *child);
    void updateChildren();
    
    bool checkForLoops();
    
private:
    void init();
};

#pragma GCC visibility pop