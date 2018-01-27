#include "ee3/NodeTranslateState.h"

#include <ee0/SubjectMgr.h>

#include <node3/Camera.h>
#include <node3/Viewport.h>

namespace ee3
{

NodeTranslateState::NodeTranslateState(const n3::Camera& cam, const n3::Viewport& vp, 
	                                   ee0::SubjectMgr& sub_mgr, const ee::SelectionSet<n3::SceneNode>& selection)
	: m_cam(cam)
	, m_vp(vp)
	, m_sub_mgr(sub_mgr)
	, m_selection(selection)
{
}

void NodeTranslateState::OnMousePress(const sm::vec2& pos)
{
	m_first_pos = m_last_pos = pos;
}

void NodeTranslateState::OnMouseRelease(const sm::vec2& pos)
{
	m_sub_mgr.NotifyObservers(ee0::MSG_UPDATE_COMPONENTS);
}

void NodeTranslateState::OnMouseDrag(const sm::vec2& pos)
{
	if (m_selection.IsEmpty()) {
		return;
	}

	Translate(m_last_pos, pos);
	m_last_pos = pos;

	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void NodeTranslateState::Translate(const sm::vec2& first, const sm::vec2& curr)
{
	m_selection.Traverse(Visitor(m_cam, m_vp, first, curr));
}

//////////////////////////////////////////////////////////////////////////
// class NodeTranslateState::Visitor
//////////////////////////////////////////////////////////////////////////

void NodeTranslateState::Visitor::
Visit(const n3::SceneNodePtr& node, bool& next)
{
	auto& ctrans = node->GetComponent<n3::CompTransform>();

	float dist = m_cam.GetToward().Dot(ctrans.GetPosition() - m_cam.GetPos());

	sm::vec3 last = m_vp.TransPos3ScreenToDir(m_last, m_cam).Normalized() * dist;
	sm::vec3 curr = m_vp.TransPos3ScreenToDir(m_curr, m_cam).Normalized() * dist;
	ctrans.Translate(curr - last);

	next = true;
}

}