#include "editop/NodeRotateState.h"

namespace eone
{

NodeRotateState::NodeRotateState(const ee::SelectionSet<SceneNode>& selection)
	: m_selection(selection)
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
}

void NodeRotateState::Rotate(const sm::vec2& start, const sm::vec2& end)
{
	m_selection.Traverse(Visitor(start, end));
}

//////////////////////////////////////////////////////////////////////////
// class NodeRotateState::Visitor
//////////////////////////////////////////////////////////////////////////

void NodeRotateState::Visitor::
Visit(const SceneNodePtr& node, bool& next)
{
////	auto cam_mat = m_canvas.GetCamera().GetRotateMat().Inverted();
//	sm::vec2 center = m_canvas.TransPos3ProjectToScreen(node->GetPos());
//	sm::vec2 base = m_canvas.TransPos3ProjectToScreen(sm::vec3(0, 0, 0));
//
//	auto& vp = m_canvas.GetViewport();
//   	sm::vec3 start = vp.MapToSphere(base + m_start -  center);
//   	sm::vec3 end   = vp.MapToSphere(base + m_end - center);
//	//sm::vec3 start = vp.MapToSphere(m_start);
//	//sm::vec3 end = vp.MapToSphere(m_end);
//
//	//auto cam_mat = m_canvas.GetCamera().GetRotateMat().Inverted();
//	//start = cam_mat * start;
//	//end   = cam_mat * end;
//		
//   	sm::Quaternion delta = sm::Quaternion::CreateFromVectors(start, end);
//	node->Rotate(delta);
//
//	next = true;
}

}