#pragma once

#include <ee/Visitor.h>
#include <ee/SelectionSet.h>

#include <ee0/EditOpState.h>

#include <node0/SceneNode.h>

namespace ee0 { class SubjectMgr; }
namespace n3 { class Camera; class Viewport; }

namespace ee3
{

class NodeTranslateState : public ee0::EditOpState
{
public:
	NodeTranslateState(const n3::Camera& cam, const n3::Viewport& vp, 
		ee0::SubjectMgr& sub_mgr, const ee::SelectionSet<n0::SceneNode>& selection);

	virtual bool OnMousePress(int x, int y) override;
	virtual bool OnMouseRelease(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

private:
	void Translate(const sm::ivec2& first, const sm::ivec2& curr);

private:
	class Visitor : public ee::RefVisitor<n0::SceneNode>
	{
	public:
		Visitor(const n3::Camera& cam, const n3::Viewport& vp, 
			const sm::ivec2& last, const sm::ivec2& curr)
			: m_cam(cam), m_vp(vp), m_last(last), m_curr(curr) {}
		virtual void Visit(const n0::SceneNodePtr& node, bool& next) override;

	private:
		const n3::Camera&   m_cam;
		const n3::Viewport& m_vp;

		sm::ivec2 m_last, m_curr;

	}; // Visitor

private:
	const n3::Camera&   m_cam;
	const n3::Viewport& m_vp;

	ee0::SubjectMgr& m_sub_mgr;

	const ee::SelectionSet<n0::SceneNode>& m_selection;

	sm::ivec2 m_last_pos;

}; // NodeTranslateState

}
