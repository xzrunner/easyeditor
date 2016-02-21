#ifndef _EASYCOMPLEX_NODE_TO_SPRITE_H_
#define _EASYCOMPLEX_NODE_TO_SPRITE_H_


#include <easyrespacker.h>

namespace ecomplex
{

class NodeToSprite
{
public:
	static ee::Sprite* Trans(const erespacker::IPackNode* node);

};	// NodeToSprite

}

#endif // _EASYCOMPLEX_NODE_TO_SPRITE_H_