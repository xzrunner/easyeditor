#pragma once

#include "AbstractEditCMPT.h"

namespace d2d
{
	class UniversalCMPT : public AbstractEditCMPT
	{
	public:
		UniversalCMPT(wxWindow* parent, const wxString& name,
			EditPanel* editPanel, AbstractEditOP* editOP);

	protected:
		virtual wxSizer* initLayout();

	}; // UniversalCMPT
}

