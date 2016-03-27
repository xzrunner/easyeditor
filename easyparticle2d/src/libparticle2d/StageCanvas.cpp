#include "StageCanvas.h"
#include "StagePanel.h"
#include "ParticleSystem.h"

#include <ee/FetchAllVisitor.h>
#include <ee/SpriteRenderer.h>
#include <ee/EE_RVG.h>
#include <ee/color_config.h>
#include <ee/Sprite.h>

namespace eparticle2d
{

static const int SCREEN_WIDTH = 1024;
static const int SCREEN_HEIGHT = 768;

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
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
	ee::RVG::Color(ee::LIGHT_RED);
	ee::RVG::Rect(ee::Vector(0, 0), SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, false);
}

}