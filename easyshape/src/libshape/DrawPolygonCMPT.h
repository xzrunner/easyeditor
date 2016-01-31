#ifndef _LIBSHAPE_DRAW_POLYGON_CMPT_H_
#define _LIBSHAPE_DRAW_POLYGON_CMPT_H_

#include "EditPolylineOP.h"
#include "DrawPolygonOP.h"

namespace libshape
{

class DrawPolygonCMPT : public ee::OneFloatValueCMPT
{
public:
	DrawPolygonCMPT(wxWindow* parent, const wxString& name, wxWindow* stage_wnd,
		ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapesImpl,
		ee::PropertySettingPanel* property);

	virtual void updateControlValue();

protected:
	virtual wxSizer* InitLayout();

private:
	wxSizer* initPreviewButtonPanel();

	void onSetColor(wxCommandEvent& event);
	void onChangeFillingType(wxCommandEvent& event);
	void onTriggerFillingColor(wxCommandEvent& event);

	void fillingButton();

private:
	wxWindow* m_stage_wnd;

	ee::MultiShapesImpl* m_shapesImpl;

	wxColour m_color;
//	wxColourData& m_colorData;

	ee::Bitmap m_bitmap;
	std::string m_filePath;

	wxButton* m_btnReview;

	wxRadioBox* m_fillingTypeChoice;

	wxButton* m_btnTrigger;

}; // DrawPolygonCMPT

}

#endif // _LIBSHAPE_DRAW_POLYGON_CMPT_H_