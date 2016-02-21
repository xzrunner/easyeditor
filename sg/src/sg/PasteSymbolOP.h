#ifndef _SG_PASTE_SPRITE_OP_H_
#define _SG_PASTE_SPRITE_OP_H_

#include <ee/PasteSymbolOP.h>

namespace sg
{

class StagePanel;
class PasteSymbolOP : public ee::PasteSymbolOP
{
public:
	PasteSymbolOP(StagePanel* stage, ee::LibraryPanel* library);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;

private:
	bool isCurrSymbolValid() const;
	bool isCurrSymbolIsWall() const;

}; // PasteSymbolOP

}

#endif // _SG_PASTE_SPRITE_OP_H_