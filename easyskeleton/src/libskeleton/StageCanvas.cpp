#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/SpriteRenderer.h>
#include <ee/EditPanelImpl.h>

#include <sprite2/Skeleton.h>

namespace libskeleton
{

StageCanvas::StageCanvas(StagePanel* editpanel, wxGLContext* glctx)
	: ee::CameraCanvas(editpanel, editpanel->GetStageImpl(), gum::CAM_ORTHO2D, glctx)
	, m_sk(editpanel->GetSkeleton())
{
	if (m_sk) {
		m_sk->AddReference();
	}
}

StageCanvas::~StageCanvas()
{
	if (m_sk) {
		m_sk->RemoveReference();
	}
}

void StageCanvas::OnDrawSprites() const
{
	m_sk->Draw(s2::RenderParams());

	m_stage->DrawEditOP();
}

}