#ifndef _EASYEDITOR_PASTE_SYMBOL_RANDOM_OP_H_
#define _EASYEDITOR_PASTE_SYMBOL_RANDOM_OP_H_

#include "PasteSymbolOP.h"
#include "PasteSymbolRandomWidget.h"

namespace ee
{

class PasteSymbolRandomOP : public PasteSymbolOP
{
public:
	PasteSymbolRandomOP(wxWindow* wnd, EditPanelImpl* stage, 
		LibraryPanel* library, PasteSymbolRandomWidget* randomWidget);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);

	virtual bool OnDraw() const;

private:
	void ChangeRandomValue();

private:
	PasteSymbolRandomWidget* m_random_widget;

	PasteSymbolRandomWidget::RandomValue m_random_val;

}; // PasteSymbolRandomOP

}

#endif // _EASYEDITOR_PASTE_SYMBOL_RANDOM_OP_H_