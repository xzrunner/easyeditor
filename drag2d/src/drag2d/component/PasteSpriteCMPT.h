#pragma once

#include "AbstractEditCMPT.h"

namespace d2d
{
	class MultiSpritesImpl;
	class PropertySettingPanel;

	class PasteSpriteCMPT : public AbstractEditCMPT
	{
	public:
		PasteSpriteCMPT(wxWindow* parent, const wxString& name, EditPanel* editPanel, 
			MultiSpritesImpl* spritesImpl, PropertySettingPanel* propertyPanel = NULL);

		bool isHorMirror() const { return m_xMirror->IsChecked(); }
		bool isVerMirror() const { return m_yMirror->IsChecked(); }

	protected:
		virtual wxSizer* initLayout();

	private:
		wxCheckBox* m_xMirror;
		wxCheckBox* m_yMirror;

	}; // PasteSpriteCMPT
}

