#ifndef _LIBSKELETON_ROTATE_JOINT_STATE_H_
#define _LIBSKELETON_ROTATE_JOINT_STATE_H_

#include <ee/ArrangeSpriteState.h>

#include <SM_Vector.h>

namespace libskeleton
{

class Joint;

class RotateJointState : public ee::ArrangeSpriteState
{
public:
	RotateJointState(Joint* joint, const sm::vec2& first_pos);
	virtual ~RotateJointState();

	virtual void OnMouseRelease(const sm::vec2& pos);
	virtual	bool OnMouseDrag(const sm::vec2& pos);

private:
	void Rotate(const sm::vec2& dst);

private:
	Joint* m_joint;
	
	sm::vec2 m_first_pos, m_last_pos;

	float m_angle;

}; // RotateJointState

}

#endif // _LIBSKELETON_ROTATE_JOINT_STATE_H_