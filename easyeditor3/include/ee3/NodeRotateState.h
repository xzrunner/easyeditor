#pragma once

#include <ee/Visitor.h>
#include <ee/SelectionSet.h>

#include <ee0/EditOpState.h>

#include <node3/SceneNode.h>

namespace ee0 { class SubjectMgr; }
namespace n3 { class Camera; class Viewport; }

namespace ee3
{

class NodeRotateState : public ee0::EditOpState
{
public:
	NodeRotateState(const n3::Camera& cam, const n3::Viewport& vp, 
		ee0::SubjectMgr& sub_mgr, const ee::SelectionSet<n3::SceneNode>& selection);

	virtual void OnMousePress(const sm::vec2& pos) override;
	virtual void OnMouseRelease(const sm::vec2& pos) override;
	virtual void OnMouseDrag(const sm::vec2& pos) override;

private:
	void Rotate(const sm::vec2& start, const sm::vec2& end);

private:
	class Visitor : public ee::RefVisitor<n3::SceneNode>
	{
	public:
		Visitor(const n3::Camera& cam, const n3::Viewport& vp, 
			const sm::vec2& start, const sm::vec2& end)
			: m_cam(cam), m_vp(vp), m_start(start), m_end(end) {}
		virtual void Visit(const n3::SceneNodePtr& node, bool& next) override;

	private:
		sm::vec2 TransPos3ProjectToScreen(const sm::vec3& proj) const;

	private:
		const n3::Camera&   m_cam;
		const n3::Viewport& m_vp;

		sm::vec2 m_start, m_end;

	}; // Visitor

private:
	const n3::Camera&   m_cam;
	const n3::Viewport& m_vp;

	ee0::SubjectMgr& m_sub_mgr;

	const ee::SelectionSet<n3::SceneNode>& m_selection;

	sm::vec2 m_last_pos;

}; // NodeRotateState

}