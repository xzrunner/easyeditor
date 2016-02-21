#ifndef _EASYEDITOR_PASTE_SYMBOL_PHYSICS_RANDOM_H_
#define _EASYEDITOR_PASTE_SYMBOL_PHYSICS_RANDOM_H_

#include "PasteSymbolPhysicsOP.h"
#include "PasteSymbolRandomWidget.h"

namespace ee
{

class PasteSymbolPhysicsRandomOP : public PasteSymbolPhysicsOP
{
public:
	PasteSymbolPhysicsRandomOP(wxWindow* stage_wnd, EditPanelImpl* stage, 
		LibraryPanel* library, PhysicsPanelImpl* physicsImpl, 
		PasteSymbolRandomWidget* randomWidget);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);

	virtual bool OnDraw() const;

private:
	void changeRandomValue();

private:
	PasteSymbolRandomWidget* m_random_widget;

	PasteSymbolRandomWidget::RandomValue m_random_value;

}; // PasteSymbolPhysicsRandomOP

}

#endif // _EASYEDITOR_PASTE_SYMBOL_PHYSICS_RANDOM_H_