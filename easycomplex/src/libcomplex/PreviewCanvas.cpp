#include "PreviewCanvas.h"

#include <easyanim.h>

namespace ecomplex
{

BEGIN_EVENT_TABLE(PreviewCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, PreviewCanvas::onTimer)
END_EVENT_TABLE()

PreviewCanvas::PreviewCanvas(d2d::EditPanel* stage, d2d::PlayControl& control,
							 const std::vector<const d2d::ISprite*>& sprites)
	: d2d::OrthoCanvas(stage)
	, m_timer(this, TIMER_ID)
	, m_control(control)
	, m_sprites(sprites)
{
	m_timer.Start(10);
}

void PreviewCanvas::initGL()
{
	d2d::OrthoCanvas::initGL();
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->getSymbol().reloadTexture();
	}
}

void PreviewCanvas::onDraw()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		const d2d::ISprite* sprite = m_sprites[i];
		const d2d::FontBlankSymbol* font = 
			dynamic_cast<const d2d::FontBlankSymbol*>(&sprite->getSymbol());
		if (font) {
			continue;
		}

// 		if (!sprite->visiable)
// 			continue;
		d2d::SpriteDraw::drawSprite(sprite, sprite->multiCol, sprite->addCol);
	}
}

void PreviewCanvas::onTimer(wxTimerEvent& event)
{
	bool refresh = m_control.update();
	if (refresh) {
		Refresh();
	}
}

}