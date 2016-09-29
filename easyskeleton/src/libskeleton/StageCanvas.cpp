#include "StageCanvas.h"
#include "StagePanel.h"
#include "Sprite.h"

#include <ee/SpriteRenderer.h>
#include <ee/EditPanelImpl.h>

namespace libskeleton
{

StageCanvas::StageCanvas(StagePanel* editpanel, wxGLContext* glctx)
	: ee::CameraCanvas(editpanel, editpanel->GetStageImpl(), glctx)
	, m_spr(editpanel->GetSprite())
{
	if (m_spr) {
		m_spr->AddReference();
	}
}

StageCanvas::~StageCanvas()
{
	if (m_spr) {
		m_spr->RemoveReference();
	}
}

void StageCanvas::OnDrawSprites() const
{
	ee::SpriteRenderer::Instance()->Draw(m_spr);

	m_stage->DrawEditOP();
}

}