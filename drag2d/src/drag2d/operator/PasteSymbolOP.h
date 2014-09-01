#pragma once

#include "ZoomViewOP.h"

namespace d2d
{
	class MultiSpritesImpl;
	class LibraryPanel;

	class PasteSymbolOP : public ZoomViewOP
	{
	public:
		PasteSymbolOP(EditPanel* editPanel, MultiSpritesImpl* panelImpl, 
			LibraryPanel* libraryPanel, float* pScale = NULL);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseMove(int x, int y);

		virtual bool onDraw(const Screen& scr) const;
		virtual bool clear();

	protected:
		MultiSpritesImpl* m_panelImpl;

		LibraryPanel* m_libraryPanel;

		float* m_pScale;

		Vector m_pos;

	}; // PasteSymbolOP
}

