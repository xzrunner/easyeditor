#include "StageCanvas.h"
#include "StagePanel.h"
#include "ParticleSystem.h"

#include <easyanim.h>

namespace eparticle3d
{

StageCanvas::StageCanvas(StagePanel* editPanel)
	: d2d::DynamicStageCanvas(editPanel)
	, m_stage(editPanel)
	, m_control(0.033f)
{
	m_currFrame = 1;
	m_last = -1;
}

StageCanvas::~StageCanvas()
{
}

void StageCanvas::onDraw()
{
	std::vector<d2d::ISprite*> sprites;
	static_cast<StagePanel*>(m_stage)->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];
		if (!sprite->visiable)
			continue;
		d2d::SpriteDraw::drawSprite(sprites[i]);
	}

	if (m_stage->m_ps) {
		m_stage->m_ps->draw(d2d::Matrix());
	}

	m_stage->drawEditTemp();
}

void StageCanvas::OnTimer()
{
	if (m_last == -1) {
		m_last = clock();
	} else {
		clock_t curr = clock();
		int dt = curr - m_last;
		m_last = curr;

		if (m_stage->m_ps) {
			m_stage->m_ps->update((float)dt / CLOCKS_PER_SEC);
		}
	}

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