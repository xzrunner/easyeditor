#ifndef _LIBSKELETON_SCALE_JOINT_AOP_H_
#define _LIBSKELETON_SCALE_JOINT_AOP_H_

#include <ee/AtomicOP.h>

#include <SM_Vector.h>

namespace ee { class Sprite; }

namespace libskeleton
{

class Joint;

class ScaleJointAOP : public ee::AtomicOP
{
public:
	ScaleJointAOP(Joint* joint, const sm::vec2& scale);
	virtual ~ScaleJointAOP();

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<ee::Sprite*>& sprs) const { return Json::Value(); }

private:
	Joint* m_joint;

	sm::vec2 m_scale;

}; // ScaleJointAOP

}

#endif // _LIBSKELETON_SCALE_JOINT_AOP_H_