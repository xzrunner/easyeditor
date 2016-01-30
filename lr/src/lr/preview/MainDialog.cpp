#include "MainDialog.h"
#include "StageCanvas.h"
#include "CtrlCamOP.h"

#include "frame/config.h"

namespace lr
{
namespace preview
{

MainDialog::MainDialog(wxWindow* parent, int width, int height,
					   const std::vector<const d2d::Sprite*>& sprites)
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

	d2d::Camera* cam = stage->GetCamera();
	float old_scale = cam->GetScale();
	cam->SetScale(old_scale / PREVIEW_SCALE);
	d2d::SetCanvasDirtySJ::Instance()->SetDirty();

	stage->SetCanvas(new StageCanvas(stage, stage->GetStageImpl(), m_control, m_sprites));
	stage->SetEditOP(new CtrlCamOP(stage, stage->GetStageImpl()));
	sizer->Add(stage, 1, wxEXPAND);	

	SetSizer(sizer);
}

}
}