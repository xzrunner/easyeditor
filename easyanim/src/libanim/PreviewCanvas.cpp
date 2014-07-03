#include "PreviewCanvas.h"
#include "tools.h"

namespace anim
{

BEGIN_EVENT_TABLE(PreviewCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, PreviewCanvas::onTimer)
END_EVENT_TABLE()

PreviewCanvas::PreviewCanvas(d2d::EditPanel* stage,
							 const Symbol* symbol)
	: d2d::OrthoCanvas(stage)
	, m_timer(this, TIMER_ID)
	, m_symbol(symbol)
	, m_control(1.0f / symbol->getFPS())
{
	m_timer.Start(10);
}

PreviewCanvas::PlaySetting& PreviewCanvas::getPlaySetting()
{
	return m_setting;
}

void PreviewCanvas::initGL()
{
	d2d::OrthoCanvas::initGL();
	d2d::SymbolMgr::Instance()->traverse(d2d::ReloadTextureVisitor<d2d::ISymbol>());
}

void PreviewCanvas::onDraw()
{
	d2d::SpriteDraw::drawSprite(m_screen, m_symbol, d2d::Matrix(), d2d::Vector(0, 0));
}

void PreviewCanvas::onTimer(wxTimerEvent& event)
{
	bool refresh = false;
	if (!m_setting.isStop) {
		refresh = m_control.update();
	}

	if (m_control.frame() >= m_symbol->getMaxFrameIndex())
	{
		if (m_setting.isCirculate) {
			m_control.reset();
		}
		else {
			m_control.decrease();
		}
	}

	if (refresh) {
		Refresh();
	}
}

} // anim
