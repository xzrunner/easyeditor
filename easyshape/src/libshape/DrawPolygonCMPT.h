#ifndef _LIBSHAPE_DRAW_POLYGON_CMPT_H_
#define _LIBSHAPE_DRAW_POLYGON_CMPT_H_

#include "EditPolylineOP.h"
#include "DrawPolygonEdgeOP.h"

#include <wx/colordlg.h>

namespace libshape
{

class DrawPolygonCMPT : public d2d::OneFloatValueCMPT
{
public:
	DrawPolygonCMPT(wxWindow* parent, const wxString& name,
		d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl,
		d2d::PropertySettingPanel* propertyPanel/*, wxColourData& colorData*/);

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
	d2d::MultiShapesImpl* m_shapesImpl;

	wxColour m_color;
//	wxColourData& m_colorData;

	d2d::Bitmap m_bitmap;
	wxString m_filePath;

	wxButton* m_btnReview;

	wxRadioBox* m_fillingTypeChoice;

	wxButton* m_btnTrigger;

}; // DrawPolygonCMPT

}

#endif // _LIBSHAPE_DRAW_POLYGON_CMPT_H_