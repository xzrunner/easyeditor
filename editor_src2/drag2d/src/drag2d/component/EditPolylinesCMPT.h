#pragma once

#include "AbstractEditCMPT.h"

namespace d2d
{
	class MultiShapesImpl;
	class PropertySettingPanel;

	class EditPolylinesCMPT : public AbstractEditCMPT
	{
	public:
		EditPolylinesCMPT(wxWindow* parent, const wxString& name,
			EditPanel* editPanel, MultiShapesImpl* shapesImpl,
			PropertySettingPanel* propertyPanel);

		virtual void updateControlValue();

		float getSimplifyThreshold() const;

	protected:
		virtual wxSizer* initLayout();

	private:
		wxSizer* initSimplifyPanel();
		wxSizer* initSmoothPanel();
		wxSizer* initEditPanel();

		void onSimplifyThresholdChanged(wxSpinEvent& event);
		void onUpdateFromSimplified(wxCommandEvent& event);

		void onMergeTwoChain(wxCommandEvent& event);
		void onTranslate(wxCommandEvent& event);

	private:
		MultiShapesImpl* m_shapesImpl;

		wxSpinCtrl* m_simplifySpin;

		wxButton* m_btnMerge;

	}; // EditPolylinesCMPT
}

