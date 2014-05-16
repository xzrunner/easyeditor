#include "PreviewCanvas.h"
#include "tools.h"

namespace anim
{

BEGIN_EVENT_TABLE(PreviewCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, PreviewCanvas::onTimer)
END_EVENT_TABLE()

PreviewCanvas::PreviewCanvas(d2d::EditPanel* stage,
							 d2d::LibraryPanel* library,
							 const Symbol* symbol)
	: d2d::OrthoCanvas(stage)
	, m_timer(this, TIMER_ID)
	, m_library(library)
	, m_symbol(symbol)
	, m_control(1.0f / symbol->m_fps)
{
	m_timer.Start(10);
	m_currFrame = 1;
}

PreviewCanvas::PlaySetting& PreviewCanvas::getPlaySetting()
{
	return m_setting;
}

void PreviewCanvas::initGL()
{
	d2d::OrthoCanvas::initGL();
//	static_cast<d2d::LibraryPanel*>(Context::Instance()->library)->reloadTexture();
	m_library->reloadTexture();
}

void PreviewCanvas::onDraw()
{
	Tools::drawAnimSymbol(m_symbol, m_currFrame);
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
