#include "ToolBarPanel.h"
#include "StagePanel.h"
#include "Shadow.h"
#include "Symbol.h"

#include <easyshape.h>

namespace eshadow
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage)
	, m_stage(stage)
{
	addChild(new libshape::DrawPolygon2CMPT(this, "多边形", stage, stage, NULL));
	SetSizer(initLayout());
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
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, "半径");
		wxSizer* radius_sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);
		{
			wxSpinCtrl* spin = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(120, -1), wxSP_ARROW_KEYS, 5, 50, 10);
			Connect(spin->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::OnChangeRadius));
			radius_sizer->Add(spin);
		}
		sizer->Add(radius_sizer);
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
		m_stage->Refresh();
	}
}

void ToolbarPanel::OnSetOuterColor(wxCommandEvent& event)
{
	Shadow* shadow = m_stage->GetSymbol()->GetShadow();
	d2d::RGBColorSettingDlg dlg(this, NULL, shadow->GetOuterColor());
	if (dlg.ShowModal()) {
		shadow->SetOuterColer(dlg.GetColor());
		shadow->BuildFace();
		m_stage->Refresh();
	}
}

void ToolbarPanel::OnChangeRadius(wxSpinEvent& event)
{
	Shadow* shadow = m_stage->GetSymbol()->GetShadow();
	shadow->SetRadius(event.GetValue());
	shadow->BuildOutterLine();
	m_stage->Refresh();
}

}