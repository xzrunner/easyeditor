#include "ee3/NodeRotateState.h"

#include <ee0/SubjectMgr.h>

#include <node3/Camera.h>
#include <node3/Viewport.h>
#include <node3/CompTransform.h>

namespace ee3
{

NodeRotateState::NodeRotateState(const n3::Camera& cam, const n3::Viewport& vp, ee0::SubjectMgr& sub_mgr,
	                             const ee::SelectionSet<n0::SceneNode>& selection)
	: m_cam(cam)
	, m_vp(vp)
	, m_sub_mgr(sub_mgr)
	, m_selection(selection)
{
	m_last_pos.MakeInvalid();
}

bool NodeRotateState::OnMousePress(int x, int y)
{
	m_last_pos.Set(x, y);

	return false;
}

bool NodeRotateState::OnMouseRelease(int x, int y)
{
	m_sub_mgr.NotifyObservers(ee0::MSG_UPDATE_COMPONENTS);

	return false;
}

bool NodeRotateState::OnMouseDrag(int x, int y)
{
	Rotate(m_last_pos, sm::ivec2(x, y));
	m_last_pos.Set(x, y);

	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);

	return false;
}

void NodeRotateState::Rotate(const sm::ivec2& start, const sm::ivec2& end)
{
	m_selection.Traverse(Visitor(m_cam, m_vp, start, end));
}

//////////////////////////////////////////////////////////////////////////
// class NodeRotateState::Visitor
//////////////////////////////////////////////////////////////////////////

void NodeRotateState::Visitor::
Visit(const n0::SceneNodePtr& node, bool& next)
{
	auto& ctrans = node->GetComponent<n3::CompTransform>();

	sm::vec2 center = TransPos3ProjectToScreen(ctrans.GetPosition());
	sm::vec2 base = TransPos3ProjectToScreen(sm::vec3(0, 0, 0));

   	sm::vec3 start = m_vp.MapToSphere(base + sm::vec2(m_start.x, m_start.y) -  center);
   	sm::vec3 end   = m_vp.MapToSphere(base + sm::vec2(m_end.x, m_end.y) - center);

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