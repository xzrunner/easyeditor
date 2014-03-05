#pragma once

#include <drag2d.h>

namespace escale9
{
	class ResizeCMPT : public d2d::AbstractEditCMPT
	{
	public:
		ResizeCMPT(wxWindow* parent, const wxString& name,
			d2d::EditPanel* editPanel);

		void setValue(int width, int height);

	protected:
		virtual wxSizer* initLayout();

	private:
		void onChangeSize(wxSpinEvent& event);

	private:
		wxSpinCtrl *m_wSpin, *m_hSpin;

	}; // ResizeCMPT
}

