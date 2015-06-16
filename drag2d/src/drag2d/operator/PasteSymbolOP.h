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

		virtual bool OnMouseLeftDown(int x, int y);
		virtual bool OnMouseMove(int x, int y);

		virtual bool OnDraw() const;
		virtual bool Clear();

	protected:
		MultiSpritesImpl* m_panelImpl;

		LibraryPanel* m_libraryPanel;

		float* m_pScale;

		Vector m_pos;

	}; // PasteSymbolOP
}

