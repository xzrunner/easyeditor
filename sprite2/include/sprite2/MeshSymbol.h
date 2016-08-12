#ifndef _SPRITE2_MESH_SYMBOL_H_
#define _SPRITE2_MESH_SYMBOL_H_

#include "S2_Symbol.h"

#include <stdint.h>

namespace s2
{

class MeshSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	MeshSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const Sprite* spr = NULL) const { return sm::rect(0, 0); }

}; // MeshSymbol

}

#endif // _SPRITE2_MESH_SYMBOL_H_