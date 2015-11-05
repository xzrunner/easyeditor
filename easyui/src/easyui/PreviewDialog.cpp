#include "PreviewDialog.h"
#include "PreviewCanvas.h"

namespace eui
{

PreviewDialog::PreviewDialog(wxWindow* parent, int width, int height,
							 const std::vector<const d2d::ISprite*>& sprites)
	: wxDialog(parent, wxID_ANY, "Preview", wxDefaultPosition, wxSize(width, height), wxCLOSE_BOX | wxCAPTION)
	, m_sprites(sprites)
	, m_control(0.033f)
{
	InitLayout();
}

void PreviewDialog::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	d2d::EditPanel* stage = new d2d::EditPanel(this, this);
	stage->SetCanvas(new PreviewCanvas(stage, stage->GetStageImpl(), m_control, m_sprites));
	sizer->Add(stage, 1, wxEXPAND);	

	SetSizer(sizer);
}

}