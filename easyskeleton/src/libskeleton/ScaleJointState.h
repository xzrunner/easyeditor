#ifndef _LIBSKELETON_SCALE_JOINT_STATE_H_
#define _LIBSKELETON_SCALE_JOINT_STATE_H_

#include <ee/ArrangeSpriteState.h>

#include <SM_Vector.h>

namespace libskeleton
{

class Joint;

class ScaleJointState : public ee::ArrangeSpriteState
{
public:
	ScaleJointState(Joint* joint, const sm::vec2& first_pos);
	virtual ~ScaleJointState();

	virtual void OnMouseRelease(const sm::vec2& pos);
	virtual	bool OnMouseDrag(const sm::vec2& pos);

private:
	void Scale(const sm::vec2& dst);

private:
	Joint* m_joint;

	sm::vec2 m_first_pos, m_last_pos;

	sm::vec2 m_scale;

}; // ScaleJointState

}

#endif // _LIBSKELETON_SCALE_JOINT_STATE_H_