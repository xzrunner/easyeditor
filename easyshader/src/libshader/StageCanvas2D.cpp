#include "StageCanvas2D.h"
#include "StagePanel2D.h"
#include "Shader2D.h"

namespace eshader
{

static const int SCREEN_WIDTH = 1024;
static const int SCREEN_HEIGHT = 768;

BEGIN_EVENT_TABLE(StageCanvas2D, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, StageCanvas2D::OnTimer)
END_EVENT_TABLE()

StageCanvas2D::StageCanvas2D(StagePanel2D* stage)
	: d2d::OrthoCanvas(stage)
	, m_timer(this, TIMER_ID)
	, m_stage(stage)
	, m_start_time(0)
{
	m_bg_color.set(1, 1, 1, 1);
	m_timer.Start(100);
}

StageCanvas2D::~StageCanvas2D()
{
}

void StageCanvas2D::OnMousePressed(const d2d::Vector& pos)
{
	m_start_time = clock();

	Shader* shader = m_stage->GetShader();
	d2d::ISprite* sprite = m_stage->QuerySpriteByPos(pos);
	if (shader && sprite) {
		d2d::ShaderMgr::Instance()->sprite();

		d2d::Vector center = sprite->getCenter();
		d2d::Rect r = sprite->getSymbol().GetSize();
		float x = (pos.x - center.x) / r.xLength() + 0.5f,
			  y = (pos.y - center.y) / r.xLength() + 0.5f;
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
	d2d::PrimitiveDraw::rect(d2d::Matrix(), SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 
		d2d::LIGHT_RED_LINE);
}

void StageCanvas2D::DrawSprites() const
{
	std::vector<d2d::ISprite*> sprites;
	static_cast<StagePanel2D*>(m_stage)->TraverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];
		if (!sprite->visiable)
			continue;
		d2d::SpriteRenderer::Instance()->Draw(sprites[i]);
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
		d2d::ShaderMgr::Instance()->sprite();
		static_cast<Shader2D*>(shader)->SetTimeUniform(time);
		Refresh();
	}
}

}