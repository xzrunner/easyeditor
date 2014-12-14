#include "StageCanvas.h"
#include "StagePanel.h"

#include "Shader.h"

namespace eshader
{

static const int SCREEN_WIDTH = 1024;
static const int SCREEN_HEIGHT = 768;

BEGIN_EVENT_TABLE(StageCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, StageCanvas::OnTimer)
END_EVENT_TABLE()

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::OrthoCanvas(stage)
	, m_timer(this, TIMER_ID)
	, m_stage(stage)
	, m_start_time(0)
{
	m_bgColor.set(1, 1, 1, 1);
	m_timer.Start(100);
}

StageCanvas::~StageCanvas()
{
}

void StageCanvas::ResetTime()
{
	m_start_time = clock();
}

void StageCanvas::onDraw()
{
	DrawBackground();
	DrawSprites();
	m_editPanel->drawEditTemp();
}

void StageCanvas::DrawBackground() const
{
	d2d::PrimitiveDraw::rect(d2d::Matrix(), SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 
		d2d::LIGHT_RED_LINE);
}

void StageCanvas::DrawSprites() const
{
	std::vector<d2d::ISprite*> sprites;
	static_cast<StagePanel*>(m_editPanel)->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];
		if (!sprite->visiable)
			continue;
		d2d::SpriteDraw::drawSprite(sprites[i]);
	}
}

void StageCanvas::OnTimer(wxTimerEvent& event)
{
	clock_t curr = clock();
	if (m_start_time == 0) {
		m_start_time = curr;
	}
	float time = (float)(curr - m_start_time) / CLOCKS_PER_SEC;

	Shader* shader = m_stage->GetShader();
	if (shader) {
		d2d::ShaderMgr::Instance()->sprite();
		shader->UpdateTimeUniform(time);
		Refresh();
	}
}

}