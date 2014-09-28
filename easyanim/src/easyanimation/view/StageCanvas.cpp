#include "StageCanvas.h"
#include "StagePanel.h"

namespace eanim
{

BEGIN_EVENT_TABLE(StageCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, StageCanvas::onTimer)
END_EVENT_TABLE()

StageCanvas::StageCanvas(d2d::EditPanel* stage)
	: d2d::OrthoCanvas(stage)
	, m_timer(this, TIMER_ID)
	, m_background(NULL)
{
	m_timer.Start(1000 / 30);
}

StageCanvas::~StageCanvas()
{
	if (m_background)
	{
		m_background->release();
	}
}

void StageCanvas::onDraw()
{
	drawbackground();
	std::vector<d2d::ISprite*> sprites;
	static_cast<StagePanel*>(m_editPanel)->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites), d2d::e_visible);
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		d2d::SpriteDraw::drawSprite(m_screen, sprites[i]);
	m_editPanel->drawEditTemp(m_screen);
}

void StageCanvas::onTimer(wxTimerEvent& event)
{
	Refresh();
}

void StageCanvas::drawbackground() const
{
	if (m_background) {
		m_background->draw(m_screen, d2d::Matrix(), m_background->getRegion());
	}

	float xedge = GetSize().GetWidth() * 0.5f;
	float yedge = GetSize().GetHeight() * 0.5f;
	d2d::PrimitiveDraw::cross(d2d::Vector(0,0), xedge, yedge, d2d::LIGHT_GREY);
}

} // eanim