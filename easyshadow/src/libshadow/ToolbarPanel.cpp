#include "ToolBarPanel.h"
#include "StagePanel.h"
#include "Shadow.h"
#include "Symbol.h"

#include <easyshape.h>

namespace eshadow
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage->GetStageImpl())
	, m_stage(stage)
{
	addChild(new libshape::DrawPolygon2CMPT(this, "多边形", stage, stage->GetStageImpl(), stage, NULL, NULL));
	SetSizer(initLayout());
}

void ToolbarPanel::SetValue(int key, const d2d::UICallback::Data& data)
{
	switch (key)
	{
	case S_RADIUS:
		{
			Shadow* shadow = m_stage->GetSymbol()->GetShadow();
			shadow->SetRadius(data.val0);
			shadow->BuildOutterLine();
			m_stage->SetCanvasDirty();
		}
		break;
	}
}

void ToolbarPanel::GetValue(int key, d2d::UICallback::Data& data)
{
	switch (key)
	{
	case S_RADIUS:
		int zz = 0;
		break;
	}
}

wxSizer* ToolbarPanel::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(20);
	sizer->Add(initChildrenLayout());
	sizer->AddSpacer(40);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, "内环颜色");
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(ToolbarPanel::OnSetInnerColor));
		sizer->Add(btn);
	}
	{
		wxButton* btn = new wxButton(this, wxID_ANY, "外环颜色");
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(ToolbarPanel::OnSetOuterColor));
		sizer->Add(btn);
	}
	sizer->AddSpacer(20);
	{
		m_slider_radius = new d2d::SliderCtrlOne(this, "半径", "radius",
			this, S_RADIUS, d2d::SliderItem("", "", 20, 5, 200));
		sizer->Add(m_slider_radius);
	}
	return sizer;
}

void ToolbarPanel::OnSetInnerColor(wxCommandEvent& event)
{
	Shadow* shadow = m_stage->GetSymbol()->GetShadow();
	d2d::RGBColorSettingDlg dlg(this, NULL, shadow->GetInnerColor());
	if (dlg.ShowModal()) {
		shadow->SetInnerColer(dlg.GetColor());
		shadow->BuildFace();
		m_stage->SetCanvasDirty();
	}
}

void ToolbarPanel::OnSetOuterColor(wxCommandEvent& event)
{
	Shadow* shadow = m_stage->GetSymbol()->GetShadow();
	d2d::RGBColorSettingDlg dlg(this, NULL, shadow->GetOuterColor());
	if (dlg.ShowModal()) {
		shadow->SetOuterColer(dlg.GetColor());
		shadow->BuildFace();
		m_stage->SetCanvasDirty();
	}
}

}