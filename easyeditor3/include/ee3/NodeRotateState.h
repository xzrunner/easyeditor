#pragma once

#include <ee0/EditOpState.h>
#include <ee0/SelectionSet.h>

#include <node0/typedef.h>
#include <SM_Vector.h>

namespace ee0 { class SubjectMgr; }
namespace pt3 { class Camera; class Viewport; }

namespace ee3
{

class NodeRotateState : public ee0::EditOpState
{
public:
	NodeRotateState(const pt3::Camera& cam, const pt3::Viewport& vp, 
		ee0::SubjectMgr& sub_mgr, const ee0::SelectionSet<n0::SceneNode>& selection);

	virtual bool OnMousePress(int x, int y) override;
	virtual bool OnMouseRelease(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

private:
	void Rotate(const sm::ivec2& start, const sm::ivec2& end);

	sm::vec2 TransPos3ProjectToScreen(const sm::vec3& proj) const;

private:
	const pt3::Camera&   m_cam;
	const pt3::Viewport& m_vp;

	ee0::SubjectMgr& m_sub_mgr;

	const ee0::SelectionSet<n0::SceneNode>& m_selection;

	sm::ivec2 m_last_pos;

}; // NodeRotateState

}