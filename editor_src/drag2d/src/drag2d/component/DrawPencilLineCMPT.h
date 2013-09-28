#pragma once

#include "AbstractEditCMPT.h"

namespace d2d
{
	class MultiShapesImpl;

	class DrawPencilLineCMPT : public AbstractEditCMPT
	{
	public:
		DrawPencilLineCMPT(wxWindow* parent, const wxString& name,
			EditPanel* editPanel, MultiShapesImpl* shapesImpl);

		float getSimplifyThreshold() const {
			return m_thresholdSlider->GetValue() * 0.1f;
		}

	protected:
		virtual wxSizer* initLayout();

	private:
		wxSlider* m_thresholdSlider;

	}; // DrawPencilLineCMPT
}

