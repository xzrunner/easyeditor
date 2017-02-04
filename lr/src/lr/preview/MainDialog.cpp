#include "MainDialog.h"
#include "StageCanvas.h"
#include "CtrlCamOP.h"

#include "frame/config.h"

#include <ee/EditPanel.h>
#include <ee/panel_msg.h>

#include <sprite2/OrthoCamera.h>

#include <wx/sizer.h>

namespace lr
{
namespace preview
{

MainDialog::MainDialog(wxWindow* parent, wxGLContext* glctx, int width, 
					   int height, const std::vector<ee::Sprite*>& sprs)
	: wxDialog(parent, wxID_ANY, "Preview", wxDefaultPosition, wxSize(width, height), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_sprs(sprs)
	, m_control(0.033f)
{
	InitLayout(glctx);
}

void MainDialog::InitLayout(wxGLContext* glctx)
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	ee::EditPanel* stage = new ee::EditPanel(this, this);

	StageCanvas* canvas = new StageCanvas(stage, stage->GetStageImpl(), m_control, m_sprs, glctx);
	stage->SetCanvas(canvas);
	canvas->RemoveReference();

	if (canvas->GetCamera()->Type() == s2::CAM_ORTHO2D)
	{
		s2::OrthoCamera* cam = static_cast<s2::OrthoCamera*>(canvas->GetCamera());
		float old_scale = cam->GetScale();
		cam->SetScale(old_scale / PREVIEW_SCALE);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();		
	}
	
	ee::EditOP* op = new CtrlCamOP(stage, stage->GetStageImpl());
	stage->SetEditOP(op);

	sizer->Add(stage, 1, wxEXPAND);	

	SetSizer(sizer);
}

}
}