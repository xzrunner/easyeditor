#include "StageCanvas.h"
#include "StagePanel.h"
#include "ParticleSystem.h"

#include <easyanim.h>

namespace eparticle2d
{

static const int SCREEN_WIDTH = 1024;
static const int SCREEN_HEIGHT = 768;

BEGIN_EVENT_TABLE(StageCanvas, d2d::PerspectCanvas)
	EVT_TIMER(TIMER_ID, StageCanvas::onTimer)
END_EVENT_TABLE()

StageCanvas::StageCanvas(StagePanel* editPanel)
	: d2d::OrthoCanvas(editPanel)
	, m_stage(editPanel)
	, m_timer(this, TIMER_ID)
	, m_control(0.033f)
{
	m_bgColor.set(1, 1, 1, 1);

	m_timer.Start(10);
	m_currFrame = 1;
	m_last = -1;
}

StageCanvas::~StageCanvas()
{
}

void StageCanvas::onDraw()
{
	DrawBackground();

	std::vector<d2d::ISprite*> sprites;
	static_cast<StagePanel*>(m_editPanel)->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];
		if (!sprite->visiable)
			continue;
		d2d::SpriteDraw::drawSprite(sprites[i]);
	}
	
	ParticleSystem* ps = m_stage->GetParticleSystem();
	if (ps) {
		ps->Draw(d2d::Matrix());
	}

	m_editPanel->drawEditTemp();
}

void StageCanvas::onTimer(wxTimerEvent& event)
{
	if (m_last == -1) {
		m_last = clock();
	} else {
		clock_t curr = clock();
		int dt = curr - m_last;
		m_last = curr;

		ParticleSystem* ps = m_stage->GetParticleSystem();
		if (ps) {
			ps->Update((float)dt / CLOCKS_PER_SEC);
		}
	}

	std::vector<anim::Sprite*> sprites;
	static_cast<StagePanel*>(m_editPanel)->traverseSprites(d2d::FetchAllVisitor<anim::Sprite>(sprites));
	size_t max = 0;
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		max = std::max(max, sprites[i]->getSymbol().getMaxFrameIndex());

	m_control.update();
	m_currFrame = m_control.frame();
	if (m_currFrame >= max) {
		m_currFrame = 1;
		m_control.reset();
	}

	Refresh();
}

void StageCanvas::DrawBackground() const
{
	d2d::PrimitiveDraw::rect(d2d::Matrix(), SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 
		d2d::LIGHT_RED_LINE);
}

}