#pragma once

#include "AbstractEditCMPT.h"

namespace d2d
{
	class MultiShapesImpl;

	class EditNodesCMPT : public AbstractEditCMPT
	{
	public:
		EditNodesCMPT(wxWindow* parent, const wxString& name,
			EditPanel* editPanel, MultiShapesImpl* shapesImpl);

	protected:
		virtual wxSizer* initLayout();

	private:
		wxSizer* initEditPanel();

		void onSimplifyTrigger(wxCommandEvent& event);
		void onSmoothTrigger(wxCommandEvent& event);
		void onSaveEditChanged(wxCommandEvent& event);

	private:
		wxSpinCtrl* m_simplifySpin;
		wxSpinCtrl* m_smoothSpin;

	}; // EditNodesCMPT
}

