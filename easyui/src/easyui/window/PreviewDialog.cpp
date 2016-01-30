#include "PreviewDialog.h"
#include "PreviewCanvas.h"

namespace eui
{
namespace window
{

PreviewDialog::PreviewDialog(wxWindow* parent, wxGLContext* glctx, int width, 
							 int height, const std::vector<const d2d::Sprite*>& sprites)
	: wxDialog(parent, wxID_ANY, "Preview", wxDefaultPosition, wxSize(width, height), wxCLOSE_BOX | wxCAPTION)
	, m_sprites(sprites)
	, m_control(0.033f)
{
	InitLayout(glctx);
}

void PreviewDialog::InitLayout(wxGLContext* glctx)
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	d2d::EditPanel* stage = new d2d::EditPanel(this, this);
	stage->SetCanvas(new PreviewCanvas(stage, stage->GetStageImpl(), m_control, m_sprites, glctx));
	sizer->Add(stage, 1, wxEXPAND);	

	SetSizer(sizer);
}

}
}