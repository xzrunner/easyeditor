#include "StageCanvas.h"
#include "StagePanel.h"
#include "ParticleSystem.h"

#include <ee/FetchAllVisitor.h>
#include <ee/Sprite.h>
#include <ee/SpriteRenderer.h>
#include <ee/EE_RVG.h>
#include <ee/cfg_const.h>

#include <ps_3d.h>

namespace eparticle3d
{

StageCanvas::StageCanvas(StagePanel* editPanel)
	: ee::CameraCanvas(editPanel, editPanel->GetStageImpl())
	, m_stage(editPanel)
{
}

StageCanvas::~StageCanvas()
{
}

void StageCanvas::OnDrawSprites() const
{
	DrawBackground();

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

void StageCanvas::DrawBackground() const
{
	ee::RVG::Color(ee::Colorf(0.8f, 0.8f, 0.8f));
	ee::RVG::LineWidth(2);
	ee::RVG::Rect(ee::Vector(0, 0), ee::SCREEN_WIDTH, ee::SCREEN_HEIGHT, false);
}

}