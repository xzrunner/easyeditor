#ifndef _LIBSKELETON_ROTATE_JOINT_AOP_H_
#define _LIBSKELETON_ROTATE_JOINT_AOP_H_

#include <ee/AtomicOP.h>
#include <ee/Sprite.h>

#include <SM_Vector.h>

namespace libskeleton
{

class Joint;

class RotateJointAOP : public ee::AtomicOP
{
public:
	RotateJointAOP(const std::shared_ptr<Joint>& joint, float rot);
	
	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<ee::SprPtr>& sprs) const { return Json::Value(); }

private:
	std::shared_ptr<Joint> m_joint;

	float m_rot;

}; // RotateJointAOP

}

#endif // _LIBSKELETON_ROTATE_JOINT_AOP_H_