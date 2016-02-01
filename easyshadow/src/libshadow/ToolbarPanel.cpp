#include "ToolBarPanel.h"
#include "StagePanel.h"
#include "Shadow.h"
#include "Symbol.h"

#include <easyshape.h>

namespace eshadow
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
	, m_stage(stage)
{
	AddChild(new eshape::DrawPolygon2CMPT(this, "多边形", stage, stage->GetStageImpl(), stage, NULL));
	SetSizer(InitLayout());
}

void ToolbarPanel::SetValue(int key, const ee::UICallback::Data& data)
{
	switch (key)
	{
	case S_RADIUS:
		{
			Shadow* shadow = m_stage->GetSymbol()->GetShadow();
			shadow->SetRadius(data.val0);
			shadow->BuildOutterLine();
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}
		break;
	}
}

void ToolbarPanel::GetValue(int key, ee::UICallback::Data& data)
{
	switch (key)
	{
	case S_RADIUS:
		int zz = 0;
		break;
	}
}

wxSizer* ToolbarPanel::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(20);
	sizer->Add(InitChildrenLayout());
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
		m_slider_radius = new ee::SliderCtrlOne(this, "半径", "radius",
			this, S_RADIUS, ee::SliderItem("", "", 20, 5, 200));
		sizer->Add(m_slider_radius);
	}
	return sizer;
}

void ToolbarPanel::OnSetInnerColor(wxCommandEvent& event)
{
	Shadow* shadow = m_stage->GetSymbol()->GetShadow();
	ee::RGBColorSettingDlg dlg(this, NULL, shadow->GetInnerColor());
	if (dlg.ShowModal() == wxID_OK) {
		shadow->SetInnerColer(dlg.GetColor());
		shadow->BuildFace();
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void ToolbarPanel::OnSetOuterColor(wxCommandEvent& event)
{
	Shadow* shadow = m_stage->GetSymbol()->GetShadow();
	ee::RGBColorSettingDlg dlg(this, NULL, shadow->GetOuterColor());
	if (dlg.ShowModal() == wxID_OK) {
		shadow->SetOuterColer(dlg.GetColor());
		shadow->BuildFace();
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

}