//
//  GRigidTransformNode.cpp
//  GMath
//
//  Created by Caleb Jordan on 9/1/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GRigidTransformNode.h"

gRigidTransformNode::gRigidTransformNode()
: localTransform(), parentTransform(), globalTransform()
{
    init();
}

gRigidTransformNode::gRigidTransformNode(gRigidTransform &parentTransform)
: localTransform(), parentTransform(parentTransform)
{
    init();
    
    globalTransform = localTransform;
    globalTransform *= parentTransform;
}

gRigidTransformNode::gRigidTransformNode(gRigidTransform &parentTransform, gRigidTransform &localTransform)
: localTransform(localTransform), parentTransform(parentTransform)
{
    init();
    
    globalTransform = localTransform;
    globalTransform *= parentTransform;
}

#pragma mark TRANSFORMS

gRigidTransform &gRigidTransformNode::getGlobalTransform()
{
    return globalTransform;
}

void gRigidTransformNode::setGlobalTransform(gRigidTransform &newGlobal, bool update)
{
    gRigidTransform newLocal(newGlobal * parentTransform.getInverse());
    
    localTransform = newLocal;
    
    globalTransform = newGlobal;
    
    if (update && numChildren > 0)
        updateChildren();
}

gRigidTransform &gRigidTransformNode::getParentTransform()
{
    return parentTransform;
}

void gRigidTransformNode::setParentTransform(gRigidTransform &newParent, bool update)
{
    parentTransform = newParent;
    
    globalTransform = parentTransform * localTransform;
    
    if (update && numChildren > 0)
        updateChildren();
}

gRigidTransform &gRigidTransformNode::getLocalTransform()
{
    return localTransform;
}

void gRigidTransformNode::setLocalTransform(gRigidTransform &newLocal, bool update)
{
    localTransform = newLocal;
    
    globalTransform = parentTransform * localTransform;
    
    if (update && numChildren > 0)
        updateChildren();
}

#pragma mark CHILDREN AND TREE STUFF

bool gRigidTransformNode::addChild(gRigidTransformNode *child)
{
    children.push_back(child);
    child->setParentTransform(getGlobalTransform());
    
    if (checkForLoops()) {
        children.pop_back();
        return false;
    } else {
        numChildren++;
        return true;
    }
}

bool gRigidTransformNode::removeChild(gRigidTransformNode *child)
{
    for (int i = 0; i < numChildren; i++) {
        if (children[i] == child) {
            children.erase(children.begin() + i);
            numChildren--;
            return true;
        }
    }
    
    return false;
}

void gRigidTransformNode::updateChildren()
{
    for (int i = 0; i < numChildren; i++) {
        children[i]->setParentTransform(globalTransform);
    }
}

bool gRigidTransformNode::checkForLoops()
{
    if (touched) {
        touched = false;
        return true;
    }
    
    touched = true;
    for (int i = 0; i < numChildren; i++) {
        if (checkForLoops()) {
            touched = false;
            return true;
        }
    }
    
    touched = false;
    return false;
}

#pragma mark PRIVATE STUFF

void gRigidTransformNode::init()
{
    touched = false;
    numChildren = 0;
}