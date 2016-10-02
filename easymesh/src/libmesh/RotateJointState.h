#ifndef _EASYMESH_ROTATE_JOINT_STATE_H_
#define _EASYMESH_ROTATE_JOINT_STATE_H_

#include <ee/ArrangeSpriteState.h>

#include <SM_Vector.h>

namespace s2 { class MeshJoint; }

namespace emesh
{

class RotateJointState : public ee::ArrangeSpriteState
{
public:
	RotateJointState(s2::MeshJoint* joint, const sm::vec2& first_pos);
	virtual ~RotateJointState();

	virtual void OnMouseRelease(const sm::vec2& pos);
	virtual	bool OnMouseDrag(const sm::vec2& pos);

private:
	void Rotate(const sm::vec2& dst);

private:
	s2::MeshJoint* m_joint;
	
	sm::vec2 m_first_pos, m_last_pos;

	float m_angle;

}; // RotateJointState

}

#endif // _EASYMESH_ROTATE_JOINT_STATE_H_