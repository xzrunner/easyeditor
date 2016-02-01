#include "DrawPolygonCMPT.h"
#include "EditPolylineWithCopyNodeOP.h"
#include "PolygonShape.h"
#include "DrawPencilPolygonCMPT.h"

#include <ee/SelectShapesOP.h>
#include <ee/MultiShapesImpl.h>
#include <ee/ShapeSelection.h>
#include <ee/EmptyVerifyVisitor.h>
#include <ee/FetchAllVisitor.h>
#include <ee/RGBColorSettingDlg.h>
#include <ee/SymbolMgr.h>
#include <ee/panel_msg.h>
#include <ee/ImageSymbol.h>

namespace eshape
{

DrawPolygonCMPT::DrawPolygonCMPT(wxWindow* parent, const std::string& name, wxWindow* stage_wnd,
								 ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapes_impl,
								 ee::PropertySettingPanel* property)
	: ee::OneFloatValueCMPT(parent, name, stage, "node capture", 5, 30, 10)
	, m_stage_wnd(stage_wnd)
	, m_shapes_impl(shapes_impl)
	, m_color(*wxBLACK)
{
	m_editop = NULL;
	// draw polygon with pen, node capture
	{
		ee::OneFloatValueCMPT* cmpt = new ee::OneFloatValueCMPT(this, "pen", stage, "node capture", 5, 30, 10);
		ee::EditOP* op = new EditPolylineOP<DrawPolygonOP, ee::SelectShapesOP>
			(stage_wnd, stage, shapes_impl, property, cmpt, /*cmpt*/this);
		cmpt->SetEditOP(op);
		AddChild(cmpt);
	}
	// draw polygon with pencil, simplify threshold
	{
		AddChild(new DrawPencilPolygonCMPT(this, "pencil", stage_wnd, stage, shapes_impl));
	}
}

void DrawPolygonCMPT::UpdateControlValue()
{
	bool empty;
	m_shapes_impl->GetShapeSelection()->Traverse(ee::EmptyVerifyVisitor(empty));
	m_btn_trigger->Enable(!empty);
}

wxSizer* DrawPolygonCMPT::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(ee::OneFloatValueCMPT::InitLayout());

	sizer->AddSpacer(20);

	sizer->Add(InitChildrenLayout());

	sizer->AddSpacer(20);

	sizer->Add(InitPreviewButtonPanel());

	sizer->AddSpacer(10);

	m_btn_trigger = new wxButton(this, wxID_ANY, wxT("设置"));
	Connect(m_btn_trigger->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DrawPolygonCMPT::OnTriggerFillingColor));
	sizer->Add(m_btn_trigger);
	m_btn_trigger->Enable(false);

	return sizer;
}

wxSizer* DrawPolygonCMPT::InitPreviewButtonPanel()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxArrayString choices;
	choices.Add(wxT("颜色"));
	choices.Add(wxT("纹理"));
	m_filling_type_choice = new wxRadioBox(this, wxID_ANY, wxT("填充方式"), wxDefaultPosition, wxDefaultSize, choices, 1, wxRA_SPECIFY_COLS);
	Connect(m_filling_type_choice->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(DrawPolygonCMPT::OnChangeFillingType));
	sizer->Add(m_filling_type_choice);

	sizer->AddSpacer(10);

	m_btn_review = new wxButton(this, wxID_ANY);	
	Connect(m_btn_review->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DrawPolygonCMPT::OnSetColor));
	sizer->Add(m_btn_review);
	FillingButton();

	return sizer;
}

void DrawPolygonCMPT::OnSetColor(wxCommandEvent& event)
{
	switch (m_filling_type_choice->GetSelection()) 
	{
	case 0:
// 		{
// 			// todo trans between wxColor and ee::Colorf
// 			ee::Colorf col;
// 			col.r = m_color.Red() / 255.0f;
// 			col.g = m_color.Green() / 255.0f;
// 			col.b = m_color.Blue() / 255.0f;
// 			ee::HSLColorSettingDlg dlg(m_stage_wnd, NULL, col);
// 			if (dlg.ShowModal() == wxID_OK) {
// 				col = dlg.GetColor();
// 				m_color.Set(col.r * 255, col.g * 255, col.b * 255);
// 			}
// 		}
		{
			// todo trans between wxColor and ee::Colorf
			ee::Colorf col;
			col.r = m_color.Red() / 255.0f;
			col.g = m_color.Green() / 255.0f;
			col.b = m_color.Blue() / 255.0f;
			ee::RGBColorSettingDlg dlg(m_stage_wnd, NULL, col);
			if (dlg.ShowModal() == wxID_OK) {
				col = dlg.GetColor();
				m_color.Set(col.r * 255, col.g * 255, col.b * 255);
			}
		}
		break;
	case 1:
		{
			wxFileDialog dlg(this, wxT("选择纹理"), wxEmptyString, wxEmptyString, wxT("*.png;*.jpg"), wxFD_OPEN);
			if (dlg.ShowModal() == wxID_OK)
			{
				m_filepath = dlg.GetPath();
				m_bitmap.LoadFromFile(m_filepath);
			}
		}
		break;
	}
	FillingButton();
}

void DrawPolygonCMPT::OnChangeFillingType(wxCommandEvent& event)
{
	FillingButton();
}

void DrawPolygonCMPT::OnTriggerFillingColor(wxCommandEvent& event)
{
	std::vector<PolygonShape*> polys;
	m_shapes_impl->GetShapeSelection()->Traverse(ee::FetchAllVisitor<PolygonShape>(polys));

	for (size_t i = 0, n = polys.size(); i < n; ++i)
	{
		PolygonShape* poly = polys[i];
		switch (m_filling_type_choice->GetSelection()) 
		{
		case 0:
			poly->SetMaterialColor(ee::Colorf(m_color.Red() / 255.0f, m_color.Green() / 255.0f, m_color.Blue() / 255.0f));
			break;
		case 1:
			poly->SetMaterialTexture(static_cast<ee::ImageSymbol*>(ee::SymbolMgr::Instance()->FetchSymbol(m_filepath)));
			break;
		}
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void DrawPolygonCMPT::FillingButton()
{
	switch (m_filling_type_choice->GetSelection()) 
	{
	case 0:
		{
			wxImage img(m_btn_review->GetSize().GetWidth(), m_btn_review->GetSize().GetHeight());
			img.SetRGB(wxRect(m_btn_review->GetSize()), m_color.Red(), m_color.Green(), m_color.Blue());
			wxBitmap bitmap(img);
			m_btn_review->SetBitmap(bitmap);
		}
		break;
	case 1:
		if (const wxBitmap* bmp = m_bitmap.GetLargeBmp()) {
			m_btn_review->SetBitmap(*bmp);
		}
		break;
	}
}

}