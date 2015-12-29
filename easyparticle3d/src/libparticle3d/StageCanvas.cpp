#include "StageCanvas.h"
#include "StagePanel.h"
#include "ParticleSystem.h"

#include <easyanim.h>
#include <ps_3d.h>

namespace eparticle3d
{

StageCanvas::StageCanvas(StagePanel* editPanel)
	: d2d::OrthoCanvas(editPanel, editPanel->GetStageImpl())
	, m_stage(editPanel)
{
}

StageCanvas::~StageCanvas()
{
}

void StageCanvas::OnDrawSprites() const
{
	std::vector<d2d::ISprite*> sprites;
	static_cast<StagePanel*>(m_stage)->TraverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];
		if (!sprite->visiable)
			continue;
		d2d::SpriteRenderer::Instance()->Draw(sprites[i]);
	}

	if (m_stage->m_ps) {
		m_stage->m_ps->Draw(m_stage->GetPSMat());
	}

	m_stage->DrawEditOP();
}

}