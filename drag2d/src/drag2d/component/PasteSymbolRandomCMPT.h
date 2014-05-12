#pragma once

#include "AbstractEditCMPT.h"

namespace d2d
{
	class MultiSpritesImpl;
	class LibraryPanel;
	class PasteSymbolRandomWidget;

	class PasteSymbolRandomCMPT : public AbstractEditCMPT
	{
	public:
		PasteSymbolRandomCMPT(wxWindow* parent, const wxString& name,
			EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
			LibraryPanel* libraryPanel);

	protected:
		virtual wxSizer* initLayout();

	private:
		PasteSymbolRandomWidget* m_randomWidget;

	}; // PasteSymbolRandomCMPT
}

