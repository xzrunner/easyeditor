#include "MainDialog.h"
#include "StageCanvas.h"
#include "CtrlCamOP.h"

namespace lr
{
namespace preview
{

MainDialog::MainDialog(wxWindow* parent, int width, int height,
					   const std::vector<const d2d::ISprite*>& sprites)
	: wxDialog(parent, wxID_ANY, "Preview", wxDefaultPosition, wxSize(width, height), wxCLOSE_BOX | wxCAPTION)
	, m_sprites(sprites)
	, m_control(0.033f)
{
	InitLayout();
}

void MainDialog::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	d2d::EditPanel* stage = new d2d::EditPanel(this, this);
	stage->setCanvas(new StageCanvas(stage, m_control, m_sprites));
	stage->setEditOP(new CtrlCamOP(stage));
	sizer->Add(stage, 1, wxEXPAND);	

	SetSizer(sizer);
}

}
}