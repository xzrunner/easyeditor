#pragma once

#include "PasteSymbolOP.h"

#include "widgets/PasteSymbolRandomWidget.h"

namespace d2d
{
	class PasteSymbolRandomOP : public PasteSymbolOP
	{
	public:
		PasteSymbolRandomOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
			LibraryPanel* libraryPanel, PasteSymbolRandomWidget* randomWidget);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseRightDown(int x, int y);

		virtual bool onDraw(const Screen& scr) const;

	private:
		void changeRandomValue();

	private:
		PasteSymbolRandomWidget* m_randomWidget;

		PasteSymbolRandomWidget::RandomValue m_randomValue;

	}; // PasteSymbolRandomOP
}

