
#define CATCH_CONFIG_CPP11_NULLPTR

#include "ObjectPool.h"
#include "Node2d.h"
#include "Random.h"

#include "catch.hpp"

using namespace MiniEngine;
using namespace std;

TEST_CASE("Node2d_CreateGraph")
{
    ObjectPool<Node2d> pool(32);
     
    Node2d *level1_1=pool.create();
    Node2d *level2_1=pool.create();
    Node2d *level2_2=pool.create();
    
    Node2d *level3_1=pool.create();
     
    level1_1->update();
    level2_1->update();
    level2_2->update();
    level3_1->update();
    
    CHECK(level1_1->isFlagSet(Node::Dirty)==false);
    CHECK(level2_1->isFlagSet(Node::Dirty)==false); 
    CHECK(level2_2->isFlagSet(Node::Dirty)==false);
    CHECK(level3_1->isFlagSet(Node::Dirty)==false);
    
    level1_1->addChild(level2_1);
    CHECK(level2_1->isFlagSet(Node::Dirty));
    CHECK(level1_1->isFlagSet(Node::Dirty)==false);
    level1_1->addChild(level2_2);
    
    level2_2->addChild(level3_1);
    
    
    const Node2dArray *children=level1_1->children();
    CHECK(children->size()==2);
    CHECK(children->at(0)==level2_1);
    CHECK(children->at(1)==level2_2);
    CHECK(level1_1->firstChild()==level2_1);
    
    children=level2_2->children();
    CHECK(children->size()==1);
    CHECK(children->at(0)==level3_1);
    CHECK(level2_2->firstChild()==level3_1);

    children=level2_1->children();
    CHECK(children==nullptr);

    children=level3_1->children();
    CHECK(children== nullptr);
    
    
    CHECK(level1_1->parent()== nullptr);
    CHECK(level2_1->parent()==level1_1);
    CHECK(level2_1->topLevelParent()==level1_1);
    CHECK(level3_1->topLevelParent()==level1_1);
    
    Node2dArray allChildren;
    
    level1_1->collectDescendants(allChildren);
    
    CHECK(allChildren.find(level2_1)!=allChildren.end());
    CHECK(allChildren.find(level2_2)!=allChildren.end());
    CHECK(allChildren.find(level3_1)!=allChildren.end());
    
    allChildren.clear();
    
    level2_2->collectDescendants(allChildren);
    CHECK(allChildren.find(level3_1)!=allChildren.end());
    CHECK(allChildren.find(level2_2)==allChildren.end());
    
    
    CHECK(level1_1->hasChild(level2_1));
    CHECK(level1_1->hasChild(level2_2));
    CHECK(level1_1->hasChild(level3_1)==false);
    CHECK(level2_2->hasChild(level3_1));
    
}

TEST_CASE("Node2d_ModifyGraph")
{
    ObjectPool<Node2d> pool(32);
    
    Node2d *level1_1=pool.create();
    Node2d *level2_1=pool.create();
    Node2d *level2_2=pool.create();
    
    Node2d *level3_1=pool.create();
    
    
    level1_1->addChild(level2_1);
    level1_1->addChild(level2_2);
    
    level2_2->addChild(level3_1);
    
    
    Node2d *node=level2_2->removeChild(level3_1);
    CHECK(node==level3_1);
    
    level1_1->addChild(node);
    CHECK(level1_1->hasChild(level3_1));
    CHECK(level3_1->parent()==level1_1);
    
    level1_1->removeAllChildren();
    
    CHECK(level1_1->children()->size()==0);
    CHECK(level2_1->parent()==nullptr);
    CHECK(level2_2->parent()== nullptr);
    
}

TEST_CASE("Node2d_Attributes")
{
    Node2d *node=new Node2d();
    
    CHECK(node->children()== nullptr);
    CHECK(node->parent()== nullptr);
    CHECK(node->userDataAsPtr()==nullptr);
    CHECK(node->userDataAsInt()== 0);
    CHECK(node->isFlagSet(Node::InheritOrientation));
    CHECK(node->isFlagSet(Node::InheritPosition));
    CHECK(node->isFlagSet(Node::InheritScale));
    CHECK(node->isFlagSet(Node::Dirty));
    
    CHECK(node->position()==Vector2::Zero);
    CHECK(node->scale()==Vector2::One);
    CHECK(MathUtils::AlmostEqualComplement(node->orientation(), 0));

    CHECK(node->derivedPosition()==Vector2::Zero);
    CHECK(node->derivedScale()==Vector2::One);
    CHECK(MathUtils::AlmostEqualComplement(node->derivedOrientation(), 0));
    
    
    node->update();
    CHECK(node->isFlagSet(Node::Dirty)==false);
    
    node->setUserDataAs(65537);
    CHECK(node->userDataAsInt()==65537);

    node->setUserDataAs(node);
    CHECK(node->userDataAsPtr()==node);
    CHECK(node->isFlagSet(Node::Dirty)==false);
    
    node->clearFlag(Node::InheritOrientation|Node::InheritScale);
    CHECK(node->isFlagSet(Node::InheritScale)==false);
    CHECK(node->isFlagSet(Node::InheritOrientation)==false);
    CHECK(node->isFlagSet(Node::Dirty));
    
    node->update();
    node->setOrientation(45.0f);
    CHECK(node->isFlagSet(Node::Dirty));
    CHECK(MathUtils::AlmostEqualComplement(node->orientation(), 45.0f));

    node->update();
    node->setPosition(Vector2(1,2));
    CHECK(node->isFlagSet(Node::Dirty));
    CHECK(node->position()==Vector2(1,2));
    
    node->update();
    node->setScale(Vector2(-1,2));
    CHECK(node->isFlagSet(Node::Dirty));
    CHECK(node->scale()==Vector2(-1,2));
    
    node->update();
    node->rotate(45.0f);
    CHECK(node->isFlagSet(Node::Dirty));
    CHECK(MathUtils::AlmostEqualComplement(node->orientation(), 90.0f));
 
    
    node->reset();
    Transform2d transform(Vector2(10,20),65,Vector2(2,3));
    node->setLocal(transform);
    CHECK(MathUtils::AlmostEqualComplement(node->orientation(), 65));
    CHECK(node->position()==Vector2(10,20));
    CHECK(node->scale()==Vector2(2,3));

    float posArray[2]={12,15};
    float scaleArray[2]={0.5,2};
    float orientArray[1]={39};
    
    node->reset();
    node->setLocal(posArray,scaleArray,orientArray);
    CHECK(MathUtils::AlmostEqualComplement(node->orientation(), 39));
    CHECK(node->position()==Vector2(12,15));
    CHECK(node->scale()==Vector2(0.5,2));
    
    delete node;
    
} 

TEST_CASE("Node2d_Translation")
{
    Node2d *node=new Node2d();

    node->update();
    
    Vector2 translateBy=Vector2(100,0);
    
    node->setOrientation(-90.0f);
    node->translate(translateBy, Node::World);

    CHECK(node->isFlagSet(Node::Dirty));
    CHECK(node->position()==translateBy);
    node->update();
    CHECK(node->derivedPosition()==translateBy);
    
    node->translate(translateBy, Node::World);
    CHECK(node->position()==(translateBy*2));
    node->update();
    CHECK(node->derivedPosition()==(translateBy*2));
    
    
    Node2d *parent=new Node2d();
    
    parent->setPosition(50,0);
    parent->setOrientation(90.0f);
    parent->addChild(node);
    parent->setScale(1,2);
    parent->update();
    CHECK(node->position()==(translateBy*2));
    CHECK(node->derivedPosition()==Vector2(50,200));
    
    node->translate(translateBy, Node::Local);;
    node->update();
    CHECK(node->position()==Vector2(200,-100));
    CHECK(node->derivedPosition()==Vector2(250,200));
    
    node->translate(translateBy, Node::Parent);;
    CHECK(node->position()==Vector2(300,-100));
    node->update();
    CHECK(node->derivedPosition()==Vector2(250,300));
    
    delete node;
    delete parent;
    
}

TEST_CASE("Node2d_Rotation")
{  
    Node2d *node=new Node2d();
    node->translate(100,100);
    node->setOrientation(-49);
     
    Node2d *parent=new Node2d();
    
    parent->setPosition(100,100);
    parent->setOrientation(-27);
    parent->addChild(node);
    parent->update();
    
    CHECK(node->derivedPosition()==Vector2(234.499695f,143.701599f));
    CHECK(MathUtils::AlmostEqualComplement(node->derivedOrientation(),-75.9999924f));

    
    delete node;
    delete parent;
    
}


TEST_CASE("Node2d_Scale")
{
    Node2d *node=new Node2d();
    
    node->translate(10,10);
    node->setScale(2, 2);
    
    Node2d *parent=new Node2d();
    
    parent->setPosition(100,100);
    parent->setScale(2, 2);
    parent->addChild(node);
    parent->update();
    
    CHECK(node->position()==Vector2(10,10));
    CHECK(node->derivedPosition()==Vector2(120,120));
    CHECK(node->derivedScale()==Vector2(4,4));

    delete node;
    delete parent;
    
}  

TEST_CASE("Node2d_LookAt")
{
    Node2d *node=new Node2d();
    
    node->translate(100,0);
    Node2d *parent=new Node2d();
     
    parent->setPosition(100,100);
    parent->setScale(2, 2);
    parent->setOrientation(45);
    parent->addChild(node);
    parent->update();
    
    node->lookAt(Vector2(150,200), Node::Parent);
    CHECK(MathUtils::AlmostEqualComplement(node->orientation(),-14.0362425f));

    node->lookAt(Vector2(0,0), Node::World);
    CHECK(MathUtils::AlmostEqualComplement(node->orientation(),90));

    // lookAt in Local depends on current orientation since it is basically asking  ( in this case with Vector2(100,0) to  "look to my right"
    node->setOrientation(0);
    node->lookAt(Vector2(100,0), Node::Local);
    CHECK(MathUtils::AlmostEqualComplement(node->orientation(),-90));
    
    delete node;
    delete parent;
}

TEST_CASE("Node2d_transformBy")
{
    Node2d *node=new Node2d();
    node->translate(50, 50);
    node->setOrientation(45);
    
    Transform2d transform(Vector2(100,100),90,Vector2(2,2));
    node->transformBy(transform);
    CHECK(MathUtils::AlmostEqualComplement(node->orientation(),135));
    CHECK(node->position()==Vector2(-0.00000762939453f,200));
    CHECK(node->scale()==Vector2(2,2));
    
    delete node;
}

TEST_CASE("Node2d_cloneMove")
{
    ObjectPool<Node2d> pool(32);

    Node2d *node=pool.create();
    node->setPosition(Vector2(10,15));
    node->setOrientation(45.0f);
    node->clearFlag(Node::InheritOrientation|Node::InheritScale);
    Node2d *child=pool.create();
    node->addChild(child);
    Node2d *cloned=new Node2d();
    Node2d *clonedChild= pool.create();
    cloned->addChild(clonedChild);

    cloned->cloneFrom(*node);
    CHECK(cloned->derived.position==node->derived.position);
    CHECK(cloned->derived.orientation==node->derived.orientation);
    CHECK(cloned->derived.scale==node->derived.scale);
    CHECK(cloned->local.position==node->local.position);
    CHECK(cloned->local.orientation==node->local.orientation);
    CHECK(cloned->local.scale==node->local.scale);
    CHECK(cloned->isFlagSet(Node::InheritScale) ==node->isFlagSet(Node::InheritScale));
    CHECK(cloned->isFlagSet(Node::InheritOrientation) ==node->isFlagSet(Node::InheritOrientation));
    CHECK(cloned->isFlagSet(Node::InheritPosition) ==node->isFlagSet(Node::InheritPosition));
    CHECK(cloned->firstChild()==clonedChild);

    Node2d *movedFrom=pool.create();
    Node2d *movedFromChild=pool.create();

    movedFrom->addChild(movedFromChild);
    movedFrom->setScale(Vector2(2.0f,1.5f));


    *node=std::move(*movedFrom);
    CHECK(movedFrom->derived.position==node->derived.position);
    CHECK(movedFrom->derived.orientation==node->derived.orientation);
    CHECK(movedFrom->derived.scale==node->derived.scale);
    CHECK(movedFrom->local.position==node->local.position);
    CHECK(movedFrom->local.orientation==node->local.orientation);
    CHECK(movedFrom->local.scale==node->local.scale);
    CHECK(movedFrom->isFlagSet(Node::InheritScale) ==node->isFlagSet(Node::InheritScale));
    CHECK(movedFrom->isFlagSet(Node::InheritOrientation) ==node->isFlagSet(Node::InheritOrientation));
    CHECK(movedFrom->isFlagSet(Node::InheritPosition) ==node->isFlagSet(Node::InheritPosition));
    CHECK(movedFrom->children()== nullptr);
    CHECK(node->firstChild()==movedFromChild);
    CHECK(movedFromChild->parent()==node);


    node=new Node2d(std::move(*cloned));

    CHECK(cloned->derived.position==node->derived.position);
    CHECK(cloned->derived.orientation==node->derived.orientation);
    CHECK(cloned->derived.scale==node->derived.scale);
    CHECK(cloned->local.position==node->local.position);
    CHECK(cloned->local.orientation==node->local.orientation);
    CHECK(cloned->local.scale==node->local.scale);
    CHECK(cloned->isFlagSet(Node::InheritScale) ==node->isFlagSet(Node::InheritScale));
    CHECK(cloned->isFlagSet(Node::InheritOrientation) ==node->isFlagSet(Node::InheritOrientation));
    CHECK(cloned->isFlagSet(Node::InheritPosition) ==node->isFlagSet(Node::InheritPosition));
    CHECK(cloned->children()== nullptr);
    CHECK(node->firstChild()==clonedChild);
    CHECK(clonedChild->parent()==node);


    delete node;
}
