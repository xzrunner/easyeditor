#include "PreviewCanvas.h"

namespace coceditor
{

BEGIN_EVENT_TABLE(PreviewCanvas, ee::OrthoCanvas)
	EVT_TIMER(TIMER_ID, PreviewCanvas::onTimer)
END_EVENT_TABLE()

PreviewCanvas::PreviewCanvas(ee::EditPanel* stage, ee::LibraryPanel* library,
							 const std::vector<ee::ISprite*>& sprites)
	: ee::OrthoCanvas(stage)
	, m_timer(this, TIMER_ID)
	, m_library(library)
	, m_sprites(sprites)
{
	m_timer.Start(1000 / 24);
	m_currFrame = 1;

	m_maxFrame = 0;
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		if (ee::AnimSprite* anim = dynamic_cast<ee::AnimSprite*>(m_sprites[i]))
			m_maxFrame = std::max(m_maxFrame, anim->getSymbol().getMaxFrameIndex());
}

void PreviewCanvas::initGL()
{
	ee::OrthoCanvas::initGL();
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
		if (ee::AnimSprite* anim = dynamic_cast<ee::AnimSprite*>(m_sprites[i]))
		{
			std::vector<ee::ISprite*> sprites;
			getAnimChildren(anim, sprites);
			for (size_t i = 0, n = sprites.size(); i < n; ++i)
				ee::SpriteDraw::drawSprite(sprites[i]);
		}
		else
			ee::SpriteDraw::drawSprite(m_sprites[i]);
	}
}

void PreviewCanvas::getAnimChildren(ee::AnimSprite* anim, std::vector<ee::ISprite*>& sprites) const
{
	const ee::AnimSymbol& symbol = anim->getSymbol();
	for (size_t i = 0, n = symbol.m_layers.size(); i < n; ++i)
	{
		ee::AnimSymbol::Layer* layer = symbol.m_layers[i];

		ee::AnimSymbol::Frame *currFrame = getCurrFrame(layer, m_currFrame);
		if (!currFrame)
			continue;

		for (size_t i = 0, n = currFrame->sprites.size(); i < n; ++i)
			sprites.push_back(currFrame->sprites[i]);
	}
}

ee::AnimSymbol::Frame* PreviewCanvas::getCurrFrame(ee::AnimSymbol::Layer* layer, int index)
{
	for (size_t i = 0, n = layer->frames.size(); i < n; ++i)
	{
		ee::AnimSymbol::Frame* frame = layer->frames[i];
		if (frame->index > index)
			return frame;
	}
	return NULL;
}
} // coceditor