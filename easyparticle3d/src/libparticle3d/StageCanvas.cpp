#include "StageCanvas.h"
#include "StagePanel.h"
#include "ParticleSystem.h"

#include <easyanim.h>
#include <ps_3d.h>

namespace eparticle3d
{

StageCanvas::StageCanvas(StagePanel* editPanel)
	: ee::OrthoCanvas(editPanel, editPanel->GetStageImpl())
	, m_stage(editPanel)
{
}

StageCanvas::~StageCanvas()
{
}

void StageCanvas::OnDrawSprites() const
{
	std::vector<ee::Sprite*> sprites;
	static_cast<StagePanel*>(m_stage)->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		ee::Sprite* sprite = sprites[i];
		if (!sprite->visiable)
			continue;
		ee::SpriteRenderer::Instance()->Draw(sprites[i]);
	}

	if (m_stage->m_ps) {
		m_stage->m_ps->Draw(m_stage->GetPSMat());
	}

	m_stage->DrawEditOP();
}

}