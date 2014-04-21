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
{
	// todo: not 1000?
	m_timer.Start(500 / symbol->m_fps);
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
	if (!m_setting.isStop)
		++m_currFrame;

	if (m_currFrame >= m_symbol->getMaxFrameIndex())
	{
		if (m_setting.isCirculate) 
			m_currFrame = 1;
		else 
			--m_currFrame;
	}

	Refresh();
}
} // anim
