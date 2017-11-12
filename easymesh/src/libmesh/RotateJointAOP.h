#ifndef _EASYMESH_ROTATE_JOINT_AOP_H_
#define _EASYMESH_ROTATE_JOINT_AOP_H_

#include <ee/AtomicOP.h>
#include <ee/Sprite.h>

#include <SM_Vector.h>

namespace s2 { class MeshJoint; }

namespace emesh
{

class RotateJointAOP : public ee::AtomicOP
{
public:
	RotateJointAOP(s2::MeshJoint* joint, float rot);
	virtual ~RotateJointAOP();
	
	virtual void Undo() override;
	virtual void Redo() override;

	virtual Json::Value Store(const std::vector<ee::SprPtr>& sprs) const override { return Json::Value(); }

private:
	s2::MeshJoint* m_joint;

	float m_rot;

}; // RotateJointAOP

}

#endif // _EASYMESH_ROTATE_JOINT_AOP_H_