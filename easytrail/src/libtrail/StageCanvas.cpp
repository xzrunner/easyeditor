#include "StageCanvas.h"
#include "StagePanel.h"
#include "MotionTrail.h"

#include <ee/Camera.h>
#include <ee/CameraMgr.h>
#include <ee/DrawSpritesVisitor.h>
#include <ee/FetchAllVisitor.h>
#include <ee/Sprite.h>
#include <ee/SpriteRenderer.h>
#include <ee/cfg_const.h>

#include <sprite2/S2_RVG.h>

namespace etrail
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
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
		if (!sprite->IsVisible())
			continue;
		ee::SpriteRenderer::Draw(sprites[i]);
	}

	MotionTrail* trail = m_stage->m_trail;
	if (trail) {
		trail->Draw(sm::mat4());
	}
// 	for (int i = 0, n = m_stage->m_trails.size(); i < n; ++i) {
// 		m_stage->m_trails[i]->Draw(sm::mat4());
// 	}

	m_stage->DrawEditOP();
}

void StageCanvas::DrawBackground() const
{
	s2::RVG::SetColor(s2::Color(204, 204, 204));
	s2::RVG::LineWidth(2);
	s2::RVG::Rect(sm::vec2(0, 0), ee::HALF_SCREEN_WIDTH, ee::HALF_SCREEN_HEIGHT, false);
}

}