#pragma once

#include "AbstractEditCMPT.h"

#include "common/Vector.h"

namespace d2d
{
	class MultiSpritesImpl;
	class LibraryPanel;

	template <typename TOperation>
	class PasteSymbolOffsetCMPT : public AbstractEditCMPT
	{
	public:
		PasteSymbolOffsetCMPT(wxWindow* parent, const wxString& name,
			EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
			LibraryPanel* libraryPanel);

		Vector getOffset() const;

	protected:
		virtual wxSizer* initLayout();

	private:
		wxSpinCtrl *m_xSpin, *m_ySpin;

	}; // PasteSymbolOffsetCMPT
}

#include "PasteSymbolOffsetCMPT.cpp"

