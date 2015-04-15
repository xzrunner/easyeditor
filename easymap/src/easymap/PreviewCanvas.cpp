#include "PreviewCanvas.h"

namespace emap
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
	m_timer.Start(100);
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
		// 		if (!sprite->visiable)
		// 			continue;
		d2d::SpriteDraw::drawSprite(sprite, d2d::Matrix(), sprite->multiCol, sprite->addCol);
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