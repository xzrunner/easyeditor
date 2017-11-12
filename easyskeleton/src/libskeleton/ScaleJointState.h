#ifndef _LIBSKELETON_SCALE_JOINT_STATE_H_
#define _LIBSKELETON_SCALE_JOINT_STATE_H_

#include <ee/ArrangeSpriteState.h>

#include <SM_Vector.h>

#include <memory>

namespace libskeleton
{

class Joint;

class ScaleJointState : public ee::ArrangeSpriteState
{
public:
	ScaleJointState(const std::shared_ptr<Joint>& joint, const sm::vec2& first_pos);
	
	virtual void OnMouseRelease(const sm::vec2& pos) override;
	virtual	bool OnMouseDrag(const sm::vec2& pos) override;

private:
	void Scale(const sm::vec2& dst);

private:
	std::shared_ptr<Joint> m_joint;

	sm::vec2 m_first_pos, m_last_pos;

	sm::vec2 m_scale;

}; // ScaleJointState

}

#endif // _LIBSKELETON_SCALE_JOINT_STATE_H_