#ifndef _SPRITE2_MESH_SYMBOL_H_
#define _SPRITE2_MESH_SYMBOL_H_

#include "Symbol.h"

#include <stdint.h>

namespace s2
{

class MeshSymbol : public Symbol
{
public:
	MeshSymbol(void* ud);

	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;

}; // MeshSymbol

}

#endif // _SPRITE2_MESH_SYMBOL_H_