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
	ee::EditOP* editop = new EditJointPoseOP(this);
	SetEditOP(editop);
	editop->RemoveReference();

	ee::StageCanvas* canvas = new StageCanvas(this, glctx);
	SetCanvas(canvas);
	canvas->RemoveReference();
}

bool StagePanel::UpdateStage()
{
	return false;
}

}