#pragma once

#include "PasteSymbolPhysicsOP.h"

#include "widgets/PasteSymbolRandomWidget.h"

namespace d2d
{
	class PasteSymbolPhysicsRandomOP : public PasteSymbolPhysicsOP
	{
	public:
		PasteSymbolPhysicsRandomOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
			LibraryPanel* libraryPanel, PhysicsPanelImpl* physicsImpl, PasteSymbolRandomWidget* randomWidget);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseRightDown(int x, int y);

		virtual bool onDraw(const Screen& scr) const;

	private:
		void changeRandomValue();

	private:
		PasteSymbolRandomWidget* m_randomWidget;

		PasteSymbolRandomWidget::RandomValue m_randomValue;

	}; // PasteSymbolPhysicsRandomOP
}

