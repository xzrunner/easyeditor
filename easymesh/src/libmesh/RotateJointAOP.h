#ifndef _EASYMESH_ROTATE_JOINT_AOP_H_
#define _EASYMESH_ROTATE_JOINT_AOP_H_

#include <ee/AtomicOP.h>

#include <SM_Vector.h>

namespace ee { class Sprite; }

namespace s2 { class MeshJoint; }

namespace emesh
{

class RotateJointAOP : public ee::AtomicOP
{
public:
	RotateJointAOP(s2::MeshJoint* joint, float rot);
	virtual ~RotateJointAOP();
	
	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<ee::Sprite*>& sprs) const { return Json::Value(); }

private:
	s2::MeshJoint* m_joint;

	float m_rot;

}; // RotateJointAOP

}

#endif // _EASYMESH_ROTATE_JOINT_AOP_H_