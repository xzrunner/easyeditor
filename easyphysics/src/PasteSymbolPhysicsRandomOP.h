#ifndef _EPHYSICS_PASTE_SYMBOL_PHYSICS_RANDOM_H_
#define _EPHYSICS_PASTE_SYMBOL_PHYSICS_RANDOM_H_

#include "PasteSymbolPhysicsOP.h"

namespace ephysics
{

class PasteSymbolPhysicsRandomOP : public PasteSymbolPhysicsOP
{
public:
	PasteSymbolPhysicsRandomOP(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
		ee::LibraryPanel* libraryPanel, PhysicsPanelImpl* physicsImpl, 
		ee::PasteSymbolRandomWidget* randomWidget);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);

	virtual bool OnDraw() const;

private:
	void changeRandomValue();

private:
	ee::PasteSymbolRandomWidget* m_randomWidget;

	ee::PasteSymbolRandomWidget::RandomValue m_randomValue;

}; // PasteSymbolPhysicsRandomOP

}

#endif // _EPHYSICS_PASTE_SYMBOL_PHYSICS_RANDOM_H_