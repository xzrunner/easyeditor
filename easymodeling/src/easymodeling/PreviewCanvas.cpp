
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
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DynamicTexAndFont::Instance()->ReloadTexture();
	}
	resetViewport();
}

void PreviewCanvas::onDraw()
{
 	static_cast<PreviewPanel*>(m_editPanel)->drawPhysics();
 
 	ephysics::DragPhysicsOP* op = static_cast<ephysics::DragPhysicsOP*>(m_editPanel->getEditOP());
 	if (op->m_mouseJoint)
 	{
 		b2Vec2 target = op->m_mouseJoint->GetAnchorB();
 		d2d::Vector first(target.x * ephysics::BOX2D_SCALE_FACTOR, target.y * ephysics::BOX2D_SCALE_FACTOR);
 
 		if (op->currPos.isValid())
 		{
 			d2d::PrimitiveDraw::drawLine(first, op->currPos, d2d::Colorf(1, 1, 1), 1);
 			d2d::PrimitiveDraw::drawCircle(op->currPos, 2, true, 2, d2d::Colorf(0, 1, 0));
 		}
 		d2d::PrimitiveDraw::drawCircle(first, 2, true, 2, d2d::Colorf(0, 1, 0));
 	}
}

void PreviewCanvas::onTimer(wxTimerEvent& event)
{
	Refresh();
}
