
#include "PreviewCanvas.h"
#include "PreviewPanel.h"
#include "Context.h"

using namespace emodeling;

BEGIN_EVENT_TABLE(PreviewCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, PreviewCanvas::onTimer)
END_EVENT_TABLE()

PreviewCanvas::PreviewCanvas(PreviewPanel* editPanel)
	: d2d::OrthoCanvas(editPanel)
	, m_timer(this, TIMER_ID)
{
	setBgColor(d2d::Colorf(0, 0, 0));
	m_timer.Start(1000 / FRAME_RATE);
}

PreviewCanvas::~PreviewCanvas()
{
	m_timer.Stop();
}

void PreviewCanvas::initGL()
{
	d2d::OrthoCanvas::initGL();
	Context::Instance()->library->reloadTexture();
}

void PreviewCanvas::onDraw()
{
	static_cast<PreviewPanel*>(m_editPanel)->drawPhysics();

	d2d::DragPhysicsOP* op = static_cast<d2d::DragPhysicsOP*>(m_editPanel->getEditOP());
	if (op->m_mouseJoint)
	{
		b2Vec2 target = op->m_mouseJoint->GetAnchorB();
		d2d::Vector first(target.x * d2d::BOX2D_SCALE_FACTOR, target.y * d2d::BOX2D_SCALE_FACTOR);

		if (op->currPos.isValid())
		{
			d2d::PrimitiveDraw::drawLine(m_screen, first, op->currPos, d2d::Colorf(1, 1, 1), 1);
			d2d::PrimitiveDraw::drawCircle(m_screen, op->currPos, 2, true, 2, d2d::Colorf(0, 1, 0));
		}
		d2d::PrimitiveDraw::drawCircle(m_screen, first, 2, true, 2, d2d::Colorf(0, 1, 0));
	}
}

void PreviewCanvas::onTimer(wxTimerEvent& event)
{
	Refresh();
}
