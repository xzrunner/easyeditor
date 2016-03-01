#include "PreviewCanvas.h"
#include "PreviewPanel.h"
#include "Context.h"

#include <ee/DragPhysicsOP.h>
#include <ee/EditPanelImpl.h>
#include <ee/physics_const.h>
#include <ee/EE_RVG.h>

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
 
 	ee::DragPhysicsOP* op = static_cast<ee::DragPhysicsOP*>(m_stage->GetEditOP());
 	if (op->m_mouseJoint)
 	{
 		b2Vec2 target = op->m_mouseJoint->GetAnchorB();
 		ee::Vector first(target.x * ee::BOX2D_SCALE_FACTOR, target.y * ee::BOX2D_SCALE_FACTOR);
 
 		if (op->currPos.IsValid())
 		{
 			ee::RVG::Line(first, op->currPos, ee::Colorf(1, 1, 1), 1);
 			ee::RVG::Circle(op->currPos, 2, true, ee::Colorf(0, 1, 0));
 		}
 		ee::RVG::Circle(first, 2, true, ee::Colorf(0, 1, 0));
 	}
}

}