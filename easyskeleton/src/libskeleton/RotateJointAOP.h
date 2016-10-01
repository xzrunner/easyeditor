#ifndef _LIBSKELETON_ROTATE_JOINT_AOP_H_
#define _LIBSKELETON_ROTATE_JOINT_AOP_H_

#include <ee/AtomicOP.h>

#include <SM_Vector.h>

namespace ee { class Sprite; }

namespace libskeleton
{

class Joint;

class RotateJointAOP : public ee::AtomicOP
{
public:
	RotateJointAOP(Joint* joint, float rot);
	virtual ~RotateJointAOP();
	
	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<ee::Sprite*>& sprs) const { return Json::Value(); }

private:
	Joint* m_joint;

	float m_rot;

}; // RotateJointAOP

}

#endif // _LIBSKELETON_ROTATE_JOINT_AOP_H_