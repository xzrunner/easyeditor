#include "StageCanvas.h"

namespace lr
{
namespace preview
{

BEGIN_EVENT_TABLE(StageCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, StageCanvas::onTimer)
END_EVENT_TABLE()

StageCanvas::StageCanvas(d2d::EditPanel* stage, d2d::PlayControl& control,
						 const std::vector<const d2d::ISprite*>& sprites)
	: d2d::OrthoCanvas(stage)
	, m_timer(this, TIMER_ID)
	, m_control(control)
	, m_sprites(sprites)
{
	m_timer.Start(100);
}

void StageCanvas::initGL()
{
	d2d::OrthoCanvas::initGL();
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->getSymbol().reloadTexture();
	}
}

void StageCanvas::onDraw()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		const d2d::ISprite* sprite = m_sprites[i];
		// 		if (!sprite->visiable)
		// 			continue;
		d2d::SpriteDraw::drawSprite(sprite, d2d::Matrix(), sprite->multiCol, sprite->addCol);
	}
}

void StageCanvas::onTimer(wxTimerEvent& event)
{
	bool refresh = m_control.update();
	if (refresh) {
		Refresh();
	}
}

}
}