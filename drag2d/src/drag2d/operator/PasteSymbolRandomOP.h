#ifndef _DRAG2D_PASTE_SYMBOL_RANDOM_OP_H_
#define _DRAG2D_PASTE_SYMBOL_RANDOM_OP_H_

#include "PasteSymbolOP.h"

#include "widgets/PasteSymbolRandomWidget.h"

namespace d2d
{

class PasteSymbolRandomOP : public PasteSymbolOP
{
public:
	PasteSymbolRandomOP(wxWindow* wnd, d2d::EditPanelImpl* stage, MultiSpritesImpl* spritesImpl, 
		LibraryPanel* libraryPanel, PasteSymbolRandomWidget* randomWidget);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);

	virtual bool OnDraw() const;

private:
	void changeRandomValue();

private:
	PasteSymbolRandomWidget* m_randomWidget;

	PasteSymbolRandomWidget::RandomValue m_randomValue;

}; // PasteSymbolRandomOP

}

#endif // _DRAG2D_PASTE_SYMBOL_RANDOM_OP_H_