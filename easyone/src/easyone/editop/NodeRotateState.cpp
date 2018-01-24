#include "editop/NodeRotateState.h"
#include "view/StageCanvas.h"
#include "msg/SubjectMgr.h"

namespace eone
{

NodeRotateState::NodeRotateState(StageCanvas& canvas, SubjectMgr& sub_mgr,
	                             const ee::SelectionSet<SceneNode>& selection)
	: m_canvas(canvas)
	, m_sub_mgr(sub_mgr)
	, m_selection(selection)
{
}

void NodeRotateState::OnMousePress(const sm::vec2& pos)
{
	m_last_pos = pos;
}

void NodeRotateState::OnMouseRelease(const sm::vec2& pos)
{
	m_sub_mgr.NotifyObservers(MSG_UPDATE_COMPONENTS);
}

void NodeRotateState::OnMouseDrag(const sm::vec2& pos)
{
	Rotate(m_last_pos, pos);
	m_last_pos = pos;

	m_sub_mgr.NotifyObservers(MSG_SET_CANVAS_DIRTY);
}

void NodeRotateState::Rotate(const sm::vec2& start, const sm::vec2& end)
{
	m_selection.Traverse(Visitor(m_canvas, start, end));
}

//////////////////////////////////////////////////////////////////////////
// class NodeRotateState::Visitor
//////////////////////////////////////////////////////////////////////////

void NodeRotateState::Visitor::
Visit(const SceneNodePtr& node, bool& next)
{
	auto& ctrans = node->GetComponent<n3::CompTransform>();

	sm::vec2 center = m_canvas.TransPos3ProjectToScreen(ctrans.GetPosition());
	sm::vec2 base = m_canvas.TransPos3ProjectToScreen(sm::vec3(0, 0, 0));

	auto& vp = m_canvas.GetViewport();
   	sm::vec3 start = vp.MapToSphere(base + m_start -  center);
   	sm::vec3 end   = vp.MapToSphere(base + m_end - center);

	auto cam_mat = m_canvas.GetCamera().GetRotateMat().Inverted();
	start = cam_mat * start;
	end   = cam_mat * end;
		
   	sm::Quaternion delta = sm::Quaternion::CreateFromVectors(start, end);
	ctrans.Rotate(-delta);

	next = true;
}

}