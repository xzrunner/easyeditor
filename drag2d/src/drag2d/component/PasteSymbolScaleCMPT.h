#pragma once

#include "AbstractEditCMPT.h"

namespace d2d
{
	class MultiSpritesImpl;
	class LibraryPanel;

	class PasteSymbolScaleCMPT : public AbstractEditCMPT
	{
	public:
		PasteSymbolScaleCMPT(wxWindow* parent, const wxString& name,
			EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
			LibraryPanel* libraryPanel);

	protected:
		virtual wxSizer* initLayout();

	private:
		void onChangeScale(wxSpinEvent& event);

	private:
		float m_scaleVal;

	}; // PasteSymbolScaleCMPT
}

