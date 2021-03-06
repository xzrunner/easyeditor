#ifndef _LIBSKELETON_TRANSLATE_JOINT_AOP_H_
#define _LIBSKELETON_TRANSLATE_JOINT_AOP_H_

#include <ee/AtomicOP.h>
#include <ee/Sprite.h>

#include <SM_Vector.h>

namespace libskeleton
{

class Joint;

class TranslateJointAOP : public ee::AtomicOP
{
public:
	TranslateJointAOP(const std::shared_ptr<Joint>& joint, const sm::vec2& trans);
	
	virtual void Undo() override;
	virtual void Redo() override;

	virtual Json::Value Store(const std::vector<ee::SprPtr>& sprs) const override { return Json::Value(); }

private:
	std::shared_ptr<Joint> m_joint;

	sm::vec2 m_trans;

}; // TranslateJointAOP

}

#endif // _LIBSKELETON_TRANSLATE_JOINT_AOP_H_