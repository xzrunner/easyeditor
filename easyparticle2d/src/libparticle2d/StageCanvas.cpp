#include "StageCanvas.h"
#include "StagePanel.h"
#include "ParticleSystem.h"

#include <easyanim.h>

namespace eparticle2d
{

static const int SCREEN_WIDTH = 1024;
static const int SCREEN_HEIGHT = 768;

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::DynamicStageCanvas(stage)
	, m_stage(stage)
	, m_control(0.033f)
{
	m_bg_color.set(1, 1, 1, 1);
	m_currFrame = 1;
}

StageCanvas::~StageCanvas()
{
}

void StageCanvas::OnDrawSprites() const
{
	DrawBackground();

	std::vector<d2d::ISprite*> sprites;
	static_cast<StagePanel*>(m_stage)->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];
		if (!sprite->visiable)
			continue;
		d2d::SpriteRenderer::Instance()->Draw(sprites[i]);
	}
	
	ParticleSystem* ps = m_stage->GetStageData()->GetPS();
	if (ps) {
		ps->Draw(d2d::Matrix());
	}

	m_stage->DrawEditOP();
}

void StageCanvas::OnTimer()
{
	UpdateParticle2d();
	UpdateAnimation();
}

void StageCanvas::DrawBackground() const
{
	d2d::PrimitiveDraw::rect(d2d::Matrix(), SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 
		d2d::LIGHT_RED_LINE);
}

void StageCanvas::UpdateParticle2d()
{
	static clock_t last_time = 0;
	if (last_time == 0) {
		last_time = clock();
	} else {
		clock_t curr_time = clock();
		float dt = (float)(curr_time - last_time) / CLOCKS_PER_SEC;
		ParticleSystem* ps = m_stage->GetStageData()->GetPS();
		if (ps) {
			ps->Update(dt);
		}
		last_time = curr_time;
	}
}

void StageCanvas::UpdateAnimation()
{
	std::vector<libanim::Sprite*> sprites;
	static_cast<StagePanel*>(m_stage)->traverseSprites(d2d::FetchAllVisitor<libanim::Sprite>(sprites));
	size_t max = 0;
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		max = std::max(max, sprites[i]->getSymbol().getMaxFrameIndex());

	m_control.update();
	m_currFrame = m_control.frame();
	if (m_currFrame >= max) {
		m_currFrame = 1;
		m_control.reset();
	}	
}

}