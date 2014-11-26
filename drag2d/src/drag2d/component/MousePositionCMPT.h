#pragma once

#include "AbstractEditCMPT.h"

namespace d2d
{
	class Vector;

	class MousePositionCMPT : public AbstractEditCMPT
	{
	public:
		MousePositionCMPT(wxWindow* parent, const wxString& name, 
			EditPanel* editPanel, bool create_op = true);

		void updatePosition(const Vector& pos);

	protected:
		virtual wxSizer* initLayout();

	private:
		wxTextCtrl *m_xText, *m_yText;

	}; // MousePositionCMPT
}

