#ifndef _DRAG2D_PASTE_SYMBOL_H_
#define _DRAG2D_PASTE_SYMBOL_H_

#include "ZoomViewOP.h"

#include "common/Vector.h"

namespace d2d
{

class LibraryPanel;

class PasteSymbolOP : public ZoomViewOP
{
public:
	PasteSymbolOP(wxWindow* wnd, d2d::EditPanelImpl* stage, 
		LibraryPanel* libraryPanel, float* pScale = NULL);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseMove(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

protected:
	LibraryPanel* m_libraryPanel;

	float* m_pScale;

	Vector m_pos;

}; // PasteSymbolOP

}

#endif // _DRAG2D_PASTE_SYMBOL_H_