#ifndef _LIBCOMPLEX_NODE_TO_SPRITE_H_
#define _LIBCOMPLEX_NODE_TO_SPRITE_H_

#include <drag2d.h>
#include <easycoco.h>

namespace ecomplex
{

class NodeToSprite
{
public:
	static d2d::ISprite* Trans(const libcoco::IPackNode* node);

};	// NodeToSprite

}

#endif // _LIBCOMPLEX_NODE_TO_SPRITE_H_