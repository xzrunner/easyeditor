#ifndef _EASYEDITOR_SPRITE_CTRL_NODE_H_
#define _EASYEDITOR_SPRITE_CTRL_NODE_H_

#include <SM_Vector.h>

namespace ee
{

class Sprite;

class SpriteCtrlNode
{
public:
	static void GetSpriteCtrlNodes(const Sprite* sprite, sm::vec2 nodes[8]);

	static void GetSpriteCtrlNodesExt(const Sprite* sprite, sm::vec2 nodes[4]);

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
 		sm::vec2 pos;
 		Type type;
		bool valid;
 
 		void setInvalid() {
 			valid = false;
 		}
 		bool isValid() {
 			return valid;
 		}
 	};

}; // SpriteCtrlNode

}

#endif // _EASYEDITOR_SPRITE_CTRL_NODE_H_