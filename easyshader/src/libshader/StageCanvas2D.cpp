#include "StageCanvas2D.h"
#include "StagePanel2D.h"
#include "Shader2D.h"

#include <ee/Sprite.h>
#include <ee/SpriteRenderer.h>
#include <ee/Symbol.h>
#include <ee/FetchAllVisitor.h>
#include <ee/color_config.h>
#include <ee/cfg_const.h>

#include <sprite2/RVG.h>

namespace eshader
{

BEGIN_EVENT_TABLE(StageCanvas2D, ee::CameraCanvas)
	EVT_TIMER(TIMER_ID, StageCanvas2D::OnTimer)
END_EVENT_TABLE()

StageCanvas2D::StageCanvas2D(StagePanel2D* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), s2::CAM_ORTHO2D)
	, m_timer(this, TIMER_ID)
	, m_stage(stage)
	, m_start_time(0)
{
	m_bg_color.FromFloat(1, 1, 1, 1);
	m_timer.Start(100);
}

StageCanvas2D::~StageCanvas2D()
{
}

void StageCanvas2D::OnMousePressed(const sm::vec2& pos)
{
	m_start_time = clock();

	Shader* shader = m_stage->GetShader();
	auto spr = m_stage->QuerySpriteByPos(pos);
	if (shader && spr) {
		ee::ShaderMgr::Instance()->SetShader(ee::ShaderMgr::SPRITE);

		sm::vec2 center = spr->GetCenter();
		sm::rect r = spr->GetSymbol()->GetBounding();
		float x = (pos.x - center.x) / r.Width() + 0.5f,
			  y = (pos.y - center.y) / r.Width() + 0.5f;
		static_cast<Shader2D*>(shader)->SetInputUniform(x, y);
	}
}

void StageCanvas2D::OnDrawSprites() const
{
	DrawBackground();
	DrawSprites();
	m_stage->DrawEditOP();
}

void StageCanvas2D::DrawBackground() const
{
	s2::RVG::SetColor(ee::LIGHT_RED);
	s2::RVG::Rect(sm::vec2(0, 0), ee::HALF_SCREEN_WIDTH, ee::HALF_SCREEN_HEIGHT, false);
}

void StageCanvas2D::DrawSprites() const
{
	std::vector<ee::SprPtr> sprs;
	static_cast<StagePanel2D*>(m_stage)->TraverseSprites(ee::FetchAllRefVisitor<ee::Sprite>(sprs));
	for (size_t i = 0, n = sprs.size(); i < n; ++i)
	{
		auto& spr = sprs[i];
		if (!spr->IsVisible())
			continue;
		ee::SpriteRenderer::Instance()->Draw(sprs[i]);
	}
}

void StageCanvas2D::OnTimer(wxTimerEvent& event)
{
	clock_t curr = clock();
	if (m_start_time == 0) {
		m_start_time = curr;
	}
	float time = (float)(curr - m_start_time) / CLOCKS_PER_SEC;

	Shader* shader = m_stage->GetShader();
	if (shader) {
		ee::ShaderMgr::Instance()->SetShader(ee::ShaderMgr::SPRITE);
		static_cast<Shader2D*>(shader)->SetTimeUniform(time);
		Refresh(true);
	}
}

}