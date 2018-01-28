#include "ee3/NodeTranslateState.h"

#include <ee0/SubjectMgr.h>

#include <node3/Camera.h>
#include <node3/Viewport.h>
#include <node3/CompTransform.h>

namespace ee3
{

NodeTranslateState::NodeTranslateState(const n3::Camera& cam, const n3::Viewport& vp, 
	                                   ee0::SubjectMgr& sub_mgr, const ee::SelectionSet<n0::SceneNode>& selection)
	: m_cam(cam)
	, m_vp(vp)
	, m_sub_mgr(sub_mgr)
	, m_selection(selection)
{
	m_last_pos.MakeInvalid();
}

bool NodeTranslateState::OnMousePress(int x, int y)
{
	m_last_pos.Set(x, y);
	return false;
}

bool NodeTranslateState::OnMouseRelease(int x, int y)
{
	m_sub_mgr.NotifyObservers(ee0::MSG_UPDATE_COMPONENTS);
	return false;
}

bool NodeTranslateState::OnMouseDrag(int x, int y)
{
	if (m_selection.IsEmpty()) {
		return false;
	}

	Translate(m_last_pos, sm::ivec2(x, y));
	m_last_pos.Set(x, y);

	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);

	return false;
}

void NodeTranslateState::Translate(const sm::ivec2& first, const sm::ivec2& curr)
{
	m_selection.Traverse(Visitor(m_cam, m_vp, first, curr));
}

//////////////////////////////////////////////////////////////////////////
// class NodeTranslateState::Visitor
//////////////////////////////////////////////////////////////////////////

void NodeTranslateState::Visitor::
Visit(const n0::SceneNodePtr& node, bool& next)
{
	auto& ctrans = node->GetComponent<n3::CompTransform>();

	float dist = m_cam.GetToward().Dot(ctrans.GetPosition() - m_cam.GetPos());

	sm::vec3 last = m_vp.TransPos3ScreenToDir(
		sm::vec2(m_last.x, m_last.y), m_cam).Normalized() * dist;
	sm::vec3 curr = m_vp.TransPos3ScreenToDir(
		sm::vec2(m_curr.x, m_curr.y), m_cam).Normalized() * dist;
	ctrans.Translate(curr - last);

	next = true;
}

}