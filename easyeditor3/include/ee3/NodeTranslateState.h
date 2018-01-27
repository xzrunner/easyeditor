#pragma once

#include <ee/Visitor.h>
#include <ee/SelectionSet.h>

#include <ee0/EditOpState.h>

#include <node3/SceneNode.h>

namespace ee0 { class SubjectMgr; }
namespace n3 { class Camera; class Viewport; }

namespace ee3
{

class NodeTranslateState : public ee0::EditOpState
{
public:
	NodeTranslateState(const n3::Camera& cam, const n3::Viewport& vp, 
		ee0::SubjectMgr& sub_mgr, const ee::SelectionSet<n3::SceneNode>& selection);

	virtual void OnMousePress(const sm::vec2& pos) override;
	virtual void OnMouseRelease(const sm::vec2& pos) override;
	virtual void OnMouseDrag(const sm::vec2& pos) override;

private:
	void Translate(const sm::vec2& first, const sm::vec2& curr);

private:
	class Visitor : public ee::RefVisitor<n3::SceneNode>
	{
	public:
		Visitor(const n3::Camera& cam, const n3::Viewport& vp, 
			const sm::vec2& last, const sm::vec2& curr)
			: m_cam(cam), m_vp(vp), m_last(last), m_curr(curr) {}
		virtual void Visit(const n3::SceneNodePtr& node, bool& next) override;

	private:
		const n3::Camera&   m_cam;
		const n3::Viewport& m_vp;

		sm::vec2 m_last, m_curr;

	}; // Visitor

private:
	const n3::Camera&   m_cam;
	const n3::Viewport& m_vp;

	ee0::SubjectMgr& m_sub_mgr;

	const ee::SelectionSet<n3::SceneNode>& m_selection;

	sm::vec2 m_first_pos, m_last_pos;

}; // NodeTranslateState

}
