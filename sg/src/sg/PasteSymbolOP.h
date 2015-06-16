#ifndef _SG_PASTE_SPRITE_OP_H_
#define _SG_PASTE_SPRITE_OP_H_

#include <drag2d.h>

namespace sg
{

class StagePanel;
class PasteSymbolOP : public d2d::PasteSymbolOP
{
public:
	PasteSymbolOP(StagePanel* stage, d2d::LibraryPanel* library);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;

private:
	bool isCurrSymbolValid() const;
	bool isCurrSymbolIsWall() const;

}; // PasteSymbolOP

}

#endif // _SG_PASTE_SPRITE_OP_H_