#include "PreviewCanvas.h"
#include "PreviewPanel.h"
#include "Context.h"

namespace emodeling
{

PreviewCanvas::PreviewCanvas(PreviewPanel* editPanel)
	: ee::OrthoCanvas(editPanel, editPanel->GetStageImpl())
	, m_stage_panel(editPanel)
{
	SetBgColor(ee::Colorf(0, 0, 0));
}

PreviewCanvas::~PreviewCanvas()
{
}

void PreviewCanvas::OnDrawSprites() const
{
 	m_stage_panel->drawPhysics();
 
 	ephysics::DragPhysicsOP* op = static_cast<ephysics::DragPhysicsOP*>(m_stage->GetEditOP());
 	if (op->m_mouseJoint)
 	{
 		b2Vec2 target = op->m_mouseJoint->GetAnchorB();
 		ee::Vector first(target.x * ephysics::BOX2D_SCALE_FACTOR, target.y * ephysics::BOX2D_SCALE_FACTOR);
 
 		if (op->currPos.IsValid())
 		{
 			ee::PrimitiveDraw::DrawLine(first, op->currPos, ee::Colorf(1, 1, 1), 1);
 			ee::PrimitiveDraw::DrawCircle(op->currPos, 2, true, 2, ee::Colorf(0, 1, 0));
 		}
 		ee::PrimitiveDraw::DrawCircle(first, 2, true, 2, ee::Colorf(0, 1, 0));
 	}
}

}