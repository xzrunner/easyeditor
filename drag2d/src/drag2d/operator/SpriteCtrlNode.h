#ifndef _DRAG2D_SPRITE_CTRL_NODE_H_
#define _DRAG2D_SPRITE_CTRL_NODE_H_

#include "common/Vector.h"

namespace d2d
{

class ISprite;

class SpriteCtrlNode
{
public:
	
	static void GetSpriteCtrlNodes(const ISprite* sprite, Vector nodes[8]);

	static void GetSpriteCtrlNodesExt(const ISprite* sprite, Vector nodes[4]);

public:
 	// 0 4 1
 	// 5   6
 	// 2 7 3
 	enum Type
 	{
 		LEFT_UP = 0,
 		RIGHT_UP,
 		LEFT_DOWN,
 		RIGHT_DOWN,
 		UP,
 		LEFT,
 		RIGHT,
 		DOWN
 	};
 
 	struct Node
 	{
 		Vector pos;
 		Type type;
 
 		void setInvalid() {
 			pos.setInvalid();
 		}
 		bool isValid() {
 			return pos.isValid();
 		}
 	};

}; // SpriteCtrlNode

}

#endif // _DRAG2D_SPRITE_CTRL_NODE_H_