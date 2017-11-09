#ifndef _LIBSKELETON_SCALE_JOINT_AOP_H_
#define _LIBSKELETON_SCALE_JOINT_AOP_H_

#include <ee/AtomicOP.h>
#include <ee/Sprite.h>

#include <SM_Vector.h>

namespace libskeleton
{

class Joint;

class ScaleJointAOP : public ee::AtomicOP
{
public:
	ScaleJointAOP(const std::shared_ptr<Joint>& joint, const sm::vec2& scale);

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<ee::SprPtr>& sprs) const { return Json::Value(); }

private:
	std::shared_ptr<Joint> m_joint;

	sm::vec2 m_scale;

}; // ScaleJointAOP

}

#endif // _LIBSKELETON_SCALE_JOINT_AOP_H_