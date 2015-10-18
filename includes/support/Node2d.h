#ifndef Node2d_h
#define Node2d_h

#include "Vector2.h"
#include "rdestl.h"
#include "fixed_vector.h"
#include "Node.h"
#include "Transform2d.h"

namespace MiniEngine
{
    class Node2d;
    
    typedef rde::fixed_vector<Node2d*,64,true>			FixedNode2dVector64;
    typedef rde::vector<Node2d*>						Node2dArray;
    
    class Node2d
    {
    private:

        NodeTree<Node2d *> mNodeData;

        //Node2d		*mParent;
        //Node2dArray *mChildren;

        union Flags
        {
            struct
            {
                unsigned int mInheritOrientation    : 1;
                unsigned int mInheritPosition       : 1;
                unsigned int mInheritScale            : 1;
                unsigned int mDirty                    : 1;
            };
            unsigned int value;
        };
        Flags mFlags;
        union UserData
        {
            int dataAsInt;
            void *dataAsPtr;
        };

        UserData mUserData;

        void markDirty()
        {
            mFlags.mDirty = 1;
        }


    public:

        friend class NodeTree<Node2d *>;

        Node2d()
        {
            reset();
        }

        Node2d(const Node2d &other) = delete;  // doesn't make sense - a child node can only have one parent
        Node2d(Node2d &&other);

        Node2d &operator=(Node2d &&other);

        Node2d &operator=(const Node2d &other) = delete; // doesn't make sense - a child node can only have one parent

        /*
         * Clone everything but the children (a child can only have one parent )
         *
         *
         */
        void cloneFrom(const Node2d &other);

        inline bool hasChild(Node2d *child)
        {
            return mNodeData.has(child);
        }

        inline void addChild(Node2d *child)
        {
            mNodeData.addChild(child,this);
            child->markDirty();
        }

        inline Node2d *removeChild(Node2d *child)
        {
            return mNodeData.remove(child);
        }

        inline void removeAllChildren()
        {
            return mNodeData.removeAll();
        }

        inline void collectDescendants(Node2dArray &nodeArray)
        {
            return mNodeData.collectDescendants(nodeArray,this);
        }

        inline const Node2dArray *children() const
        {
            return mNodeData.children();
        }

        inline Node2d *parent() const
        {
            return mNodeData.parent();
        }

        inline Node2d*	topLevelParent() const
        {
            return mNodeData.topLevelParent(const_cast<Node2d*>(this));
        }
        inline Node2d*	firstChild() const
        {
            return mNodeData.firstChild();
        }
        
        
        inline bool isFlagSet(unsigned int flag) const
        {
            return ((mFlags.value&flag)!=0);
        }
        inline void setFlag(unsigned int flag) { mFlags.value|=flag; markDirty();}
        inline void clearFlag(unsigned int flag) { mFlags.value&=(~flag); markDirty();}
        
        inline void setLocal(const Transform2d & trans) { local=trans; markDirty();}
        void setLocal(float *position, float *scale, float *degree);
        
        inline void setLocal( const Vector2 &pos, const Vector2 &scale, float degree)
        {
            local.position=pos; local.scale=scale;
            local.orientation.setRotation(degree);
            markDirty();
        }
        
        void update();
        void transformBy(const Transform2d &transform);
        
        inline void setScale(float x, float y) { local.scale.x=x; local.scale.y=y;markDirty();}
        inline void setScale(float v) { local.scale.x=v; local.scale.y=v;markDirty();}
        inline void setScale(const Vector2 &scale) { local.scale=scale; markDirty();}
        inline void setPosition(const Vector2 &pos) { local.position=pos; markDirty();}
        inline void setPosition(float x, float y) { local.position.x=x;local.position.y=y; markDirty();}
        
        void lookAt(const Vector2& targetPoint,Node::TransformSpace relativeTo=Node::World);
        void setDirection(const Vector2& dir,Node::TransformSpace relativeTo=Node::Local);
        inline void setOrientation(float degree) { local.orientation.setRotation(degree); markDirty();}
        void translate(float x, float y,  Node::TransformSpace relativeTo=Node::Parent) { translate(Vector2(x,y),relativeTo); }
        void translate(const Vector2 &trans, Node::TransformSpace relativeTo=Node::Parent);
        void rotate(float degree);
        inline float orientation() const	{ return MathUtils::RadianToDegree(local.orientationRadians());}
        inline const Vector2&		position() const	{ return local.position;}
        inline const Vector2&		scale()	const	{ return local.scale;}
        inline float			derivedOrientation() const { return MathUtils::RadianToDegree(derived.orientationRadians());}
        inline const Vector2&		derivedPosition() const { return derived.position;}
        inline const Vector2&		derivedScale() const { return derived.scale;}
        
        inline int	userDataAsInt() const { return mUserData.dataAsInt;}
        inline void*	userDataAsPtr() const { return mUserData.dataAsPtr;}
        
        inline void setUserDataAs(int val) { mUserData.dataAsInt=val;}
        inline void setUserDataAs(void *val) { mUserData.dataAsPtr=val;}
        
        
        Transform2d local;
        Transform2d derived;
        
        inline void reset()
        {
            local.reset();
            derived.reset();
            mFlags.mInheritOrientation=1;
            mFlags.mInheritPosition=1;
            mFlags.mInheritScale=1;
            mFlags.mDirty=1;
            mUserData.dataAsPtr=0;
            mNodeData.reset();
        }
        
        
        
    };
}
#endif
