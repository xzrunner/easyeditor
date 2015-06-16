#pragma once

#include "PasteSymbolOP.h"

#include "component/PasteSymbolOffsetCMPT.h"

namespace d2d
{
	class PasteSymbolCaptureOP;
	class MultiSpritesImpl;
	class LibraryPanel;

	class PasteSymbolCaptureOP : public PasteSymbolOP
	{
	public:
		PasteSymbolCaptureOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
			LibraryPanel* libraryPanel, PasteSymbolOffsetCMPT<PasteSymbolCaptureOP>* cmpt);

		virtual bool OnMouseLeftDown(int x, int y);
		virtual bool OnMouseMove(int x, int y);

		virtual bool Clear();

	private:
		PasteSymbolOffsetCMPT<PasteSymbolCaptureOP>* m_cmpt;

		bool m_bCaptured;
		Vector m_lastPos;

	}; // PasteSymbolCaptureOP
}

