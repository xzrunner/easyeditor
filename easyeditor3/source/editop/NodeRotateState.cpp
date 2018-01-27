#include "ee3/NodeRotateState.h"

#include <ee0/SubjectMgr.h>

#include <node3/Camera.h>
#include <node3/Viewport.h>

namespace ee3
{

NodeRotateState::NodeRotateState(const n3::Camera& cam, const n3::Viewport& vp, ee0::SubjectMgr& sub_mgr,
	                             const ee::SelectionSet<n3::SceneNode>& selection)
	: m_cam(cam)
	, m_vp(vp)
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
	m_sub_mgr.NotifyObservers(ee0::MSG_UPDATE_COMPONENTS);
}

void NodeRotateState::OnMouseDrag(const sm::vec2& pos)
{
	Rotate(m_last_pos, pos);
	m_last_pos = pos;

	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void NodeRotateState::Rotate(const sm::vec2& start, const sm::vec2& end)
{
	m_selection.Traverse(Visitor(m_cam, m_vp, start, end));
}

//////////////////////////////////////////////////////////////////////////
// class NodeRotateState::Visitor
//////////////////////////////////////////////////////////////////////////

void NodeRotateState::Visitor::
Visit(const n3::SceneNodePtr& node, bool& next)
{
	auto& ctrans = node->GetComponent<n3::CompTransform>();

	sm::vec2 center = TransPos3ProjectToScreen(ctrans.GetPosition());
	sm::vec2 base = TransPos3ProjectToScreen(sm::vec3(0, 0, 0));

   	sm::vec3 start = m_vp.MapToSphere(base + m_start -  center);
   	sm::vec3 end   = m_vp.MapToSphere(base + m_end - center);

	auto cam_mat = m_cam.GetRotateMat().Inverted();
	start = cam_mat * start;
	end   = cam_mat * end;
		
   	sm::Quaternion delta = sm::Quaternion::CreateFromVectors(start, end);
	ctrans.Rotate(-delta);

	next = true;
}

sm::vec2 NodeRotateState::Visitor::
TransPos3ProjectToScreen(const sm::vec3& proj) const
{
	// todo proj mat
	return m_vp.TransPos3ProjectToScreen(/*m_mat_projection * */m_cam.GetModelViewMat() * proj, m_cam);
}

}