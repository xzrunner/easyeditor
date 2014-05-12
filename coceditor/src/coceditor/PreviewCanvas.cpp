#include "PreviewCanvas.h"

namespace coceditor
{

BEGIN_EVENT_TABLE(PreviewCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, PreviewCanvas::onTimer)
END_EVENT_TABLE()

PreviewCanvas::PreviewCanvas(d2d::EditPanel* stage, d2d::LibraryPanel* library,
							 const std::vector<d2d::ISprite*>& sprites)
	: d2d::OrthoCanvas(stage)
	, m_timer(this, TIMER_ID)
	, m_library(library)
	, m_sprites(sprites)
{
	m_timer.Start(1000 / 24);
	m_currFrame = 1;

	m_maxFrame = 0;
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		if (d2d::AnimSprite* anim = dynamic_cast<d2d::AnimSprite*>(m_sprites[i]))
			m_maxFrame = std::max(m_maxFrame, anim->getSymbol().getMaxFrameIndex());
}

void PreviewCanvas::initGL()
{
	d2d::OrthoCanvas::initGL();
	m_library->reloadTexture();
}

void PreviewCanvas::onDraw()
{
	drawStageData();
}

void PreviewCanvas::onTimer(wxTimerEvent& event)
{
	++m_currFrame;
	if (m_currFrame >= m_maxFrame)
		m_currFrame = 1;

	Refresh();
}

void PreviewCanvas::drawStageData()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		if (d2d::AnimSprite* anim = dynamic_cast<d2d::AnimSprite*>(m_sprites[i]))
		{
			std::vector<d2d::ISprite*> sprites;
			getAnimChildren(anim, sprites);
			for (size_t i = 0, n = sprites.size(); i < n; ++i)
				d2d::SpriteDraw::drawSprite(sprites[i]);
		}
		else
			d2d::SpriteDraw::drawSprite(m_sprites[i]);
	}
}

void PreviewCanvas::getAnimChildren(d2d::AnimSprite* anim, std::vector<d2d::ISprite*>& sprites) const
{
	const d2d::AnimSymbol& symbol = anim->getSymbol();
	for (size_t i = 0, n = symbol.m_layers.size(); i < n; ++i)
	{
		d2d::AnimSymbol::Layer* layer = symbol.m_layers[i];

		d2d::AnimSymbol::Frame *currFrame = getCurrFrame(layer, m_currFrame);
		if (!currFrame)
			continue;

		for (size_t i = 0, n = currFrame->sprites.size(); i < n; ++i)
			sprites.push_back(currFrame->sprites[i]);
	}
}

d2d::AnimSymbol::Frame* PreviewCanvas::getCurrFrame(d2d::AnimSymbol::Layer* layer, int index)
{
	for (size_t i = 0, n = layer->frames.size(); i < n; ++i)
	{
		d2d::AnimSymbol::Frame* frame = layer->frames[i];
		if (frame->index > index)
			return frame;
	}
	return NULL;
}
} // coceditor