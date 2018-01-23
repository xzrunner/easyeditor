#include "StagePanel.h"
#include "StageCanvas.h"
#include "EditJointPoseOP.h"

#include <ee/ZoomViewOP.h>

#include <sprite2/Skeleton.h>

namespace libskeleton
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   wxGLContext* glctx, s2::Skeleton& sk)
	: EditPanel(parent, frame)
	, m_sk(sk)
{
	SetEditOP(std::make_shared<EditJointPoseOP>(this));
	SetCanvas(std::make_shared<StageCanvas>(this, glctx));
}

bool StagePanel::UpdateStage()
{
	return false;
}

}