#include "Node2d.h"

using namespace MiniEngine;


Node2d::Node2d(Node2d&& other)
{
    derived=other.derived;
    local=other.local;
    mFlags=other.mFlags;
    mUserData=other.mUserData;
    mNodeData=std::move(other.mNodeData);
    if(this->mNodeData.children()!= nullptr)
    {
        for(auto child:*mNodeData.children())
            child->mNodeData.setParent(this);
    }
}

Node2d& Node2d::operator=(Node2d&& other)
{
    if (this!=&other)
    {
        derived=other.derived;
        local=other.local;
        mFlags=other.mFlags;
        mUserData=other.mUserData;
        mNodeData=std::move(other.mNodeData);
        if(this->mNodeData.children()!= nullptr)
        {
            for(auto child:*mNodeData.children())
                child->mNodeData.setParent(this);
        }
    }
    return *this;
}

void Node2d::cloneFrom(const Node2d &other)
{
    local=other.local;
    derived=other.derived;
    mFlags=other.mFlags;
    mUserData=other.mUserData;
    markDirty();
}




void Node2d::lookAt(const Vector2& targetPoint,Node::TransformSpace relativeTo)
{
    Vector2 origin;
    switch (relativeTo)
    {
        default:
        case Node::World:
            origin =derived.position;
            break;
        case Node::Parent:
            origin = local.position;
            break;
        case Node::Local:
            origin = Vector2::Zero;
            break;
    }
    setDirection((targetPoint-origin).normalized(),relativeTo);
}
void Node2d::setDirection(const Vector2& dir,Node::TransformSpace relativeTo)
{
    
    
    Vector2 result;
    
    switch (relativeTo)
    {
        case Node::Parent:
            if (mFlags.mInheritOrientation && mNodeData.hasParent())
            {
                result=dir;
            }
            break;
        case Node::Local:
        {
            result=dir.rotatedWithComplexNumber(local.orientation);
            break;
        }
        case Node::World:
        {
            if(mFlags.mInheritOrientation && mNodeData.hasParent())
            {
                result=dir.unRotatedWithComplexNumber(mNodeData.parent()->derived.orientation);
            }
            else
                result=dir;
            break;
        }
    }
    MathUtils::SinCos(Vector2::UnitY.rotationTo(result),local.orientation.y,local.orientation.x);
    
    markDirty();
}


void Node2d::translate(const Vector2 &trans, Node::TransformSpace relativeTo)
{
    switch(relativeTo)
    {
        case Node::Local:
        {
            local.position += trans.rotatedWithComplexNumber(local.orientation);
            break;
        }
        case Node::World:
            if ( mNodeData.hasParent())
            {
                local.position += trans.unRotatedWithComplexNumber(mNodeData.parent()->derived.orientation);
            }
            else
            {
                local.position += trans;
            }
            break;
        case Node::Parent:
            local.position += trans;
            break;
    }
    markDirty();
}
void Node2d::rotate(float degree)
{
    local.orientation=local.orientation.rotatedWithComplexNumber(Vector2::fromRotation(degree));
    markDirty();
}


void Node2d::update()
{
    
    FixedNode2dVector64			stack;
    
    bool forceDirty=(bool)mFlags.mDirty;;
    
    stack.push_back(this);
    while(!stack.empty())
    {
        Node2d *cNode=stack.back();
        stack.pop_back();
        
        if( forceDirty || cNode->mFlags.mDirty)
        {
            if (cNode->mNodeData.hasParent())
            {
                const Vector2& parentOrientation=cNode->mNodeData.parent()->derived.orientation;
                if(cNode->mFlags.mInheritOrientation)
                    cNode->derived.orientation=parentOrientation.rotatedWithComplexNumber(cNode->local.orientation);
                else
                    cNode->derived.orientation = cNode->local.orientation;
                const Vector2& parentScale = cNode->mNodeData.parent()->derived.scale;
                if(cNode->mFlags.mInheritScale)
                    cNode->derived.scale=parentScale*cNode->local.scale;
                else
                    cNode->derived.scale = cNode->local.scale;
                if(cNode->mFlags.mInheritPosition)
                {
                    cNode->derived.position = (parentScale * cNode->local.position).rotatedWithComplexNumber(parentOrientation);
                    if(cNode->mFlags.mInheritPosition)
                        cNode->derived.position+=cNode->mNodeData.parent()->derivedPosition();
                }
                else
                {
                    cNode->derived.position=cNode->local.position;
                }
                
            }
            else
            {
                cNode->derived=cNode->local;
            }
            cNode->mFlags.mDirty=0;
        }
        
        if(cNode->mNodeData.children())
        {
            for(auto childNode:*cNode->mNodeData.children())
            {
                stack.push_back(childNode);
            }
        }
    }
}
void Node2d::transformBy(const Transform2d &transform)
{
    local.transformBy(transform);
    
    markDirty();
}


void Node2d::setLocal(float *position, float *scale, float *degree)
{
    local.position.x=position[0];
    local.position.y=position[1];
    local.scale.x=scale[0];
    local.scale.y=scale[1];
    local.orientation.setRotation(degree[0]);
    
    markDirty();
}