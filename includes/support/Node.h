#ifndef NODE_H
#define NODE_H

#include "Transform3d.h"
#include "rdestl.h"



namespace MiniEngine
{

    class Node3d;
    typedef rde::vector<Node3d*>						Node3dArray;
    
    class Node
    {
    public:
        enum TransformSpace
        {
            Local,
            Parent,
            World
        };
        
        enum NodeFlags
        {
            InheritOrientation=1,
            InheritPosition=2,
            InheritScale=4,
            Dirty=8,
            FixedYaw=16
        };
    };

    template<typename T>
    class NodeTree
    {
    private:
        rde::vector<T> *mChildren;
        T mParent;

    private:

        void collectDescendantsInternal(typename rde::vector<T> &nodeArray, T node)
        {
            const rde::vector<T> *children = node->children();

            if (children)
            {
                typename rde::vector<T>::const_iterator current = children->begin();
                typename rde::vector<T>::const_iterator last = children->end();

                while (current != last)
                {
                    collectDescendantsInternal(nodeArray, *current);
                    nodeArray.push_back(*current);
                    ++current;
                }
            }
        }

    public:
        inline NodeTree()
        { mChildren = nullptr; }
        inline ~NodeTree()
        { delete mChildren; }

        inline NodeTree(NodeTree&& other)
        {
            mChildren=other.mChildren;
            mParent=other.mParent;
            other.mChildren= nullptr;
            other.mParent= nullptr;
        }

        NodeTree &operator=(NodeTree &&other)
        {
            if (this!=&other)
            {
                mParent = other.mParent;
                delete mChildren;
                mChildren = other.mChildren;
                other.mChildren = nullptr;
                other.mParent = nullptr;
            }
            return *this;
        }


        NodeTree &operator=(const NodeTree &other) = delete; // doesn't make sense - a child node can only have one parent
        NodeTree(const NodeTree &other) = delete;  // doesn't make sense - a child node can only have one parent

        inline void addChild(T child, T parentNode)
        {
            if (child == parentNode)
                return;

            if (mChildren == nullptr)
                mChildren = new rde::vector<T>();

            typename  rde::vector<T>::iterator found = mChildren->find(child);
            if (found == mChildren->end())
            {
                if (child->parent())
                    child->parent()->removeChild(child);
                mChildren->push_back(child);
                child->mNodeData.setParent(parentNode);
            }
        }

        inline void setParent(T parentNode)
        {
            mParent=parentNode;
        }

        inline bool has(T child)
        {
            if(mChildren)
                return (mChildren->find(child)!=mChildren->end());
            return false;
        }

        inline T remove(T child)
        {
            if(mChildren)
            {
                typename  rde::vector<T>::iterator found=mChildren->find(child);
                if(found!=mChildren->end())
                {
                    mChildren->erase(found);
                    child->mNodeData.setParent(nullptr);
                    return child;
                }
            }
            return nullptr;
        }

        inline void removeAll()
        {
            if(mChildren)
            {
                typename  rde::vector<T>::iterator current=mChildren->begin();

                while(current!=mChildren->end())
                {
                    (*current)->mNodeData.setParent(NULL);
                    ++current;
                }
                mChildren->clear();
            }
        }

        inline void	collectDescendants(rde::vector<T> &nodeArray, T parentNode)
        {
            collectDescendantsInternal(nodeArray,parentNode);
        }

        const rde::vector<T>* children() const { return mChildren;}
        inline T	parent() const { return mParent;}
        inline bool hasParent() const { return mParent!= nullptr;}
        inline  T	topLevelParent( T parentNode) const
        {
            T current=const_cast<T>(parentNode);

            while(true)
            {
                if(!current->mNodeData.hasParent())
                    return current;
                current=current->mNodeData.parent();
            }
        }
        inline T	firstChild() const
        {
            if(mChildren)
                return mChildren->empty() ? 0:mChildren->front();
            return nullptr;
        }

        inline void reset()
        {
            if(mChildren)
                mChildren->clear();
            mParent= nullptr;
        }



    };

}

#endif