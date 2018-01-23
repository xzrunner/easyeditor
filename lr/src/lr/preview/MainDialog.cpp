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
					   int height, const std::vector<ee::SprPtr>& sprs)
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

	stage->SetCanvas(std::make_shared<StageCanvas>(stage, stage->GetStageImpl(), m_control, m_sprs, glctx));

	if (canvas->GetCamera()->Type() == s2::CAM_ORTHO2D)
	{
		auto cam = std::dynamic_pointer_cast<s2::OrthoCamera>(canvas->GetCamera());
		float old_scale = cam->GetScale();
		cam->Set(cam->GetPosition(), old_scale / PREVIEW_SCALE);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();		
	}
	
	stage->SetEditOP(std::make_shared<CtrlCamOP>(stage, stage->GetStageImpl()));

	sizer->Add(stage, 1, wxEXPAND);	

	SetSizer(sizer);
}

}
}