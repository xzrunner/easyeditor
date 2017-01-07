#ifndef _EASYSHAPE_DRAW_POLYGON_CMPT_H_
#define _EASYSHAPE_DRAW_POLYGON_CMPT_H_

#include "EditPolylineOP.h"
#include "DrawPolygonOP.h"

#include <ee/OneFloatValueCMPT.h>

namespace s2 { class Color; }
namespace ee { class Bitmap; }

namespace eshape
{

class DrawPolygonCMPT : public ee::OneFloatValueCMPT
{
public:
	DrawPolygonCMPT(wxWindow* parent, const std::string& name, wxWindow* stage_wnd,
		ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapes_impl,
		ee::PropertySettingPanel* property);

	virtual void UpdateControlValue();

protected:
	virtual wxSizer* InitLayout();

private:
	wxSizer* InitPreviewButtonPanel();

	void OnSetColor(wxCommandEvent& event);
	void OnChangeFillingType(wxCommandEvent& event);
	void OnTriggerFillingColor(wxCommandEvent& event);

	void FillingButton();

private:
	wxWindow* m_stage_wnd;

	ee::MultiShapesImpl* m_shapes_impl;

	wxColour m_color;
//	wxColourData& m_colorData;

	ee::Bitmap* m_bitmap;
	std::string m_filepath;

	wxButton* m_btn_review;

	wxRadioBox* m_filling_type_choice;

	wxButton* m_btn_trigger;

}; // DrawPolygonCMPT

}

#endif // _EASYSHAPE_DRAW_POLYGON_CMPT_H_