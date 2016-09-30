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

		m_position = m_spr->GetPosition();
		m_angle	= m_spr->GetAngle();
		m_scale = m_spr->GetScale();
		m_shear = m_spr->GetShear();
		m_offset = m_spr->GetOffset();
		m_spr->SetScale(sm::vec2(1, 1));
		m_spr->SetShear(sm::vec2(0, 0));
		m_spr->SetOffset(sm::vec2(0, 0));
		m_spr->SetPosition(sm::vec2(0, 0));
		m_spr->SetAngle(0);
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
		m_spr->SetScale(m_scale);
		m_spr->SetShear(m_shear);
		m_spr->SetOffset(m_offset);
		m_spr->SetPosition(m_position);
		m_spr->SetAngle(m_angle);

		m_spr->RemoveReference();
	}
}

bool StagePanel::UpdateStage()
{
	return false;
}

}