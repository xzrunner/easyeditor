#include "StageCanvas.h"
#include "StagePanel.h"

#include <easyanim.h>

namespace libcomplex
{
	BEGIN_EVENT_TABLE(StageCanvas, d2d::OrthoCanvas)
		EVT_TIMER(TIMER_ID, StageCanvas::onTimer)
	END_EVENT_TABLE()

	StageCanvas::StageCanvas(StagePanel* editPanel)
		: d2d::OrthoCanvas(editPanel)
		, m_timer(this, TIMER_ID)
		, m_editPanel(editPanel)
	{
		m_timer.Start(1000 / 30);
		m_currFrame = 1;

		m_bgStyle.color.set(0.8f, 0.8f, 0.8f);
		m_clipboxStyle.color.set(0, 0.8f, 0);
	}

	void StageCanvas::initGL()
	{
		d2d::OrthoCanvas::initGL();
		m_editPanel->getSymbol()->reloadTexture();
	}

	void StageCanvas::onDraw()
	{
		d2d::PrimitiveDraw::rect(d2d::Vector(0, 0), 1024 * 0.5f, 768 * 0.5f, m_bgStyle);

		std::vector<d2d::ISprite*> sprites;
		m_editPanel->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

		for (size_t i = 0, n = sprites.size(); i < n; ++i)
		{
			d2d::ISprite* sprite = sprites[i];
			if (!sprite->visiable)
				continue;
			if (d2d::AnimSprite* anim = dynamic_cast<d2d::AnimSprite*>(sprite))
			{
				d2d::SpriteDraw::begin(sprite);
				libanim::Tools::drawAnimSymbol(&anim->getSymbol(), m_currFrame);
				d2d::SpriteDraw::end(sprite);
			}
			else
				d2d::SpriteDraw::drawSprite(sprites[i]);
		}

		d2d::PrimitiveDraw::rect(m_editPanel->getSymbol()->m_clipbox, m_clipboxStyle);

		m_editPanel->drawEditTemp();
	}

	void StageCanvas::onTimer(wxTimerEvent& event)
	{
		std::vector<d2d::AnimSprite*> sprites;
		m_editPanel->traverseSprites(d2d::FetchAllVisitor<d2d::AnimSprite>(sprites));
		size_t max = 0;
		for (size_t i = 0, n = sprites.size(); i < n; ++i)
			max = std::max(max, sprites[i]->getSymbol().getMaxFrameIndex());

		++m_currFrame;
		if (m_currFrame >= max)
			m_currFrame = 1;

		Refresh();
	}
}