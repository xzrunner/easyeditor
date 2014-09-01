#include "PreviewCanvas.h"
#include "StagePanel.h"

#include "Context.h"

using namespace formation;

BEGIN_EVENT_TABLE(PreviewCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, PreviewCanvas::onTimer)
END_EVENT_TABLE()

PreviewCanvas::PreviewCanvas(d2d::EditPanel* editPanel)
	: d2d::OrthoCanvas(editPanel)
	, m_timer(this, TIMER_ID)
	, m_yOffset(0.0f)
{
	m_timer.Start(/*1000 / (FRAME_RATE*2)*/);
	m_timer.Start(1000 / FRAME_RATE);

	Context* context = Context::Instance();
	m_scale = 480.0f / context->width;
	m_offset = m_scale * context->height / context->time / FRAME_RATE;
}

PreviewCanvas::~PreviewCanvas()
{
	m_timer.Stop();
}

void PreviewCanvas::initGL()
{
	d2d::OrthoCanvas::initGL();

	glTranslatef(-480 * 0.5f, 800 * 0.5f, 0.0f);

	Context::Instance()->library->reloadTexture();
}

void PreviewCanvas::onDraw()
{
	Context::Instance()->stage->traverseSprites(DrawVisitor(m_yOffset, m_scale), d2d::e_visible);
}

void PreviewCanvas::onTimer(wxTimerEvent& event)
{
	m_yOffset += m_offset;

	glTranslatef(0.0f, -m_offset, 0.0f);

	Refresh();
}

//////////////////////////////////////////////////////////////////////////
// class PreviewCanvas::DrawVisitor
//////////////////////////////////////////////////////////////////////////

void PreviewCanvas::DrawVisitor::visit(d2d::Object* object, bool& bFetchNext)
{
	d2d::ISprite* sprite = static_cast<d2d::ISprite*>(object);

	d2d::Vector new_pos = sprite->getPosition() * m_scale;
	const int tol = 100;
	if (new_pos.y - m_yOffset < 0 + tol && new_pos.y - m_yOffset > -800 - tol)
		d2d::SpriteDraw::drawSprite(m_scr, &sprite->getSymbol(), new_pos);

	bFetchNext = true;
}