#ifndef _EPHYSICS_PASTE_SYMBOL_PHYSICS_RANDOM_H_
#define _EPHYSICS_PASTE_SYMBOL_PHYSICS_RANDOM_H_

#include "PasteSymbolPhysicsOP.h"

namespace ephysics
{

class PasteSymbolPhysicsRandomOP : public PasteSymbolPhysicsOP
{
public:
	PasteSymbolPhysicsRandomOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
		d2d::LibraryPanel* libraryPanel, PhysicsPanelImpl* physicsImpl, d2d::PasteSymbolRandomWidget* randomWidget);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);

	virtual bool OnDraw() const;

private:
	void changeRandomValue();

private:
	d2d::PasteSymbolRandomWidget* m_randomWidget;

	d2d::PasteSymbolRandomWidget::RandomValue m_randomValue;

}; // PasteSymbolPhysicsRandomOP

}

#endif // _EPHYSICS_PASTE_SYMBOL_PHYSICS_RANDOM_H_