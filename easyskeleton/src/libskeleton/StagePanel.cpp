#include "StagePanel.h"
#include "StageCanvas.h"
#include "Sprite.h"
#include "EditJointPoseOP.h"

#include <ee/ZoomViewOP.h>

namespace libskeleton
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   wxGLContext* glctx, Sprite* spr)
	: EditPanel(parent, frame)
	, m_spr(spr)
{
	if (m_spr) {
		m_spr->AddReference();
	}

	ee::EditOP* editop = new EditJointPoseOP(this);
	SetEditOP(editop);
	editop->RemoveReference();

	ee::StageCanvas* canvas = new StageCanvas(this, glctx);
	SetCanvas(canvas);
	canvas->RemoveReference();
}

StagePanel::~StagePanel()
{
	if (m_spr) {
		m_spr->RemoveReference();
	}
}

bool StagePanel::UpdateStage()
{
	return false;
}

}