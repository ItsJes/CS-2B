// FHtree main
// CS 2B/C, Foothill College, Michael Loceff, creator

#include <iostream>
#include <string>
using namespace std;
#include "FHtree.h"

template <typename Object>
class PrintObject
{
public:
   void operator()(Object obj)
   {
      cout << obj << " ";
   }
};


int main()
{
   FHtree<string> sceneTree;
   FHtreeNode<string> *tn;

   // create a scene in a room
   tn = sceneTree.addChild(NULL, "room");

   // add three objects to the scene tree
   sceneTree.addChild(tn, "Lily the canine");
   sceneTree.addChild(tn, "Miguel the human");
   sceneTree.addChild(tn, "table");

   // add some parts to Miguel
   tn = sceneTree.find("Miguel the human");

   // Miguel's left arm
   tn = sceneTree.addChild(tn, "torso");
   tn = sceneTree.addChild(tn, "left arm");
   tn =  sceneTree.addChild(tn, "left hand");
   sceneTree.addChild(tn, "thumb");
   sceneTree.addChild(tn, "index finger");
   sceneTree.addChild(tn, "middle finger");
   sceneTree.addChild(tn, "ring finger");
   sceneTree.addChild(tn, "pinky");

   // Miguel's right arm
   tn = sceneTree.find("Miguel the human");
   tn = sceneTree.find(tn, "torso");
   tn = sceneTree.addChild(tn, "right arm");
   tn =  sceneTree.addChild(tn, "right hand");
   sceneTree.addChild(tn, "thumb");
   sceneTree.addChild(tn, "index finger");
   sceneTree.addChild(tn, "middle finger");
   sceneTree.addChild(tn, "ring finger");
   sceneTree.addChild(tn, "pinky");

   // add some parts to Lily
   tn = sceneTree.find("Lily the canine");
   tn = sceneTree.addChild(tn, "torso");
   sceneTree.addChild(tn, "right front paw");
   sceneTree.addChild(tn, "left front paw");
   sceneTree.addChild(tn, "right rear paw");
   sceneTree.addChild(tn, "left rear paw");
   sceneTree.addChild(tn, "spare mutant paw");
   sceneTree.addChild(tn, "wagging tail");

   // add some parts to table
   tn = sceneTree.find("table");
   sceneTree.addChild(tn, "north east leg");
   sceneTree.addChild(tn, "north west leg");
   sceneTree.addChild(tn, "south east leg");
   sceneTree.addChild(tn, "south west leg");

   PrintObject<string> formatlessStringPrinter;
   sceneTree.traverse( formatlessStringPrinter );
   cout << endl;

   return 0;
}