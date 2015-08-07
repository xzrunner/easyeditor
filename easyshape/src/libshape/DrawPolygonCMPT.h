#ifndef _LIBSHAPE_DRAW_POLYGON_CMPT_H_
#define _LIBSHAPE_DRAW_POLYGON_CMPT_H_

#include "EditPolylineOP.h"
#include "DrawPolygonOP.h"

#include <wx/colordlg.h>

namespace libshape
{

class DrawPolygonCMPT : public d2d::OneFloatValueCMPT
{
public:
	DrawPolygonCMPT(wxWindow* parent, const wxString& name, wxWindow* stage_wnd,
		d2d::EditPanelImpl* stage, d2d::MultiShapesImpl* shapesImpl,
		d2d::PropertySettingPanel* property,d2d::ViewPanelMgr* view_panel_mgr
		/*, wxColourData& colorData*/);

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
	wxWindow* m_stage_wnd;

	d2d::MultiShapesImpl* m_shapesImpl;

	wxColour m_color;
//	wxColourData& m_colorData;

	d2d::Bitmap m_bitmap;
	std::string m_filePath;

	wxButton* m_btnReview;

	wxRadioBox* m_fillingTypeChoice;

	wxButton* m_btnTrigger;

}; // DrawPolygonCMPT

}

#endif // _LIBSHAPE_DRAW_POLYGON_CMPT_H_