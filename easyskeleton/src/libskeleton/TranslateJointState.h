#ifndef _LIBSKELETON_TRANSLATE_JOINT_STATE_H_
#define _LIBSKELETON_TRANSLATE_JOINT_STATE_H_

#include <ee/ArrangeSpriteState.h>

#include <SM_Vector.h>

#include <memory>

namespace libskeleton
{

class Joint;

class TranslateJointState : public ee::ArrangeSpriteState
{
public:
	TranslateJointState(const std::shared_ptr<Joint>& joint, const sm::vec2& first_pos);

	virtual void OnMouseRelease(const sm::vec2& pos) override;
	virtual	bool OnMouseDrag(const sm::vec2& pos) override;

private:
	void Translate(const sm::vec2& trans);

private:
	std::shared_ptr<Joint> m_joint;
	
	sm::vec2 m_first_pos, m_last_pos;

}; // TranslateJointState

}

#endif // _LIBSKELETON_TRANSLATE_JOINT_STATE_H_