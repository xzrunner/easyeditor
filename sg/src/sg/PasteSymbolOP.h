#ifndef _SG_PASTE_SPRITE_OP_H_
#define _SG_PASTE_SPRITE_OP_H_

#include <drag2d.h>

#include "SymbolRender.h"

namespace sg
{

class StagePanel;
class PasteSymbolOP : public d2d::PasteSymbolOP
{
public:
	PasteSymbolOP(StagePanel* stage, d2d::LibraryPanel* library);

	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseDrag(int x, int y);

	virtual bool onDraw() const;

private:
	bool isCurrSymbolValid() const;

	bool isCurrSymbolIsWall() const;

private:
	SymbolRender m_render;

}; // PasteSymbolOP

}

#endif // _SG_PASTE_SPRITE_OP_H_