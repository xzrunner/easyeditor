#include "NodeRotateState.h"
#include "StageCanvas.h"
#include "NodeSelection.h"

#include <ee/panel_msg.h>

#include <sprite2/ModelSprite.h>

namespace enode3d
{

NodeRotateState::NodeRotateState(const StageCanvas& canvas,
	                             const NodeSelection& selection)
	: m_canvas(canvas)
	, m_selection(selection)
{
}

void NodeRotateState::OnMousePress(const sm::vec2& pos)
{
	m_last_pos = pos;
}

void NodeRotateState::OnMouseRelease(const sm::vec2& pos)
{
}

void NodeRotateState::OnMouseDrag(const sm::vec2& pos)
{
	Rotate(m_last_pos, pos);
	m_last_pos = pos;

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void NodeRotateState::Rotate(const sm::vec2& start, const sm::vec2& end)
{
	m_selection.Traverse(Visitor(m_canvas, start, end));
}

//////////////////////////////////////////////////////////////////////////
// class NodeRotateState::Visitor
//////////////////////////////////////////////////////////////////////////

void NodeRotateState::Visitor::
Visit(const n3::NodePtr& node, bool& next)
{
//	auto cam_mat = m_canvas.GetCamera().GetRotateMat().Inverted();
	sm::vec2 center = m_canvas.TransPos3ProjectToScreen(node->GetPos());
	sm::vec2 base = m_canvas.TransPos3ProjectToScreen(sm::vec3(0, 0, 0));

	auto& vp = m_canvas.GetViewport();
   	sm::vec3 start = vp.MapToSphere(base + m_start -  center);
   	sm::vec3 end   = vp.MapToSphere(base + m_end - center);
	//sm::vec3 start = vp.MapToSphere(m_start);
	//sm::vec3 end = vp.MapToSphere(m_end);

	//auto cam_mat = m_canvas.GetCamera().GetRotateMat().Inverted();
	//start = cam_mat * start;
	//end   = cam_mat * end;
		
   	sm::Quaternion delta = sm::Quaternion::CreateFromVectors(start, end);
	node->Rotate(delta);

	next = true;
}

}