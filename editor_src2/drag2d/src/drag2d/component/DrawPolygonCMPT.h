#pragma once

#include "NodeCaptureCMPT.h"

#include "dataset/Bitmap.h"
#include "operator/EditPolylineOP.h"
#include "operator/DrawPolygonEdgeOP.h"
#include "operator/SelectShapesOP.h"

#include <wx/colordlg.h>

namespace d2d
{
	class DrawPolygonCMPT : public NodeCaptureCMPT<EditPolylineOP<DrawPolygonEdgeOP, SelectShapesOP> >
	{
	public:
		DrawPolygonCMPT(wxWindow* parent, const wxString& name,
			EditPanel* editPanel, MultiShapesImpl* shapesImpl,
			PropertySettingPanel* propertyPanel,
			wxColourData& colorData);

		virtual void updateControlValue();

	protected:
		virtual wxSizer* initLayout();

	private:
		wxSizer* initPreviewButtonPanel();

		void onSetColor(wxCommandEvent& event);
		void onChangeFillingType(wxCommandEvent& event);
		void onTriggerFillingColor(wxCommandEvent& event);

		void fillingButton();

	private:
		MultiShapesImpl* m_shapesImpl;

		wxColour m_color;
		wxColourData& m_colorData;

		Bitmap m_bitmap;
		wxString m_filePath;

		wxButton* m_btnReview;

		wxRadioBox* m_fillingTypeChoice;

		wxButton* m_btnTrigger;

	}; // DrawPolygonCMPT
}

