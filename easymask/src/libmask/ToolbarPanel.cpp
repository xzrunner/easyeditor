#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "StageCanvas.h"

#include <ee/panel_msg.h>

namespace emask
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
	, m_stage(stage)
{
	SetSizer(InitLayout());
}

wxSizer* ToolbarPanel::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxCheckBox* mask_render = new wxCheckBox(this, wxID_ANY, "mask render");
	mask_render->SetValue(false);
	Connect(mask_render->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
		wxCommandEventHandler(ToolbarPanel::OnChangeMaskRender));
	sizer->Add(mask_render, 0);

	return sizer;
}

void ToolbarPanel::OnChangeMaskRender(wxCommandEvent& event)
{
	bool mask_render = event.IsChecked();
	static_cast<StageCanvas*>(m_stage->GetCanvas())->SetMaskRender(mask_render);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}