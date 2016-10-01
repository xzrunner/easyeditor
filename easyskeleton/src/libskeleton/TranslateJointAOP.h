#ifndef _LIBSKELETON_TRANSLATE_JOINT_AOP_H_
#define _LIBSKELETON_TRANSLATE_JOINT_AOP_H_

#include <ee/AtomicOP.h>

#include <SM_Vector.h>

namespace ee { class Sprite; }

namespace libskeleton
{

class Joint;

class TranslateJointAOP : public ee::AtomicOP
{
public:
	TranslateJointAOP(Joint* joint, const sm::vec2& trans);
	virtual ~TranslateJointAOP();
	
	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<ee::Sprite*>& sprs) const { return Json::Value(); }

private:
	Joint* m_joint;

	sm::vec2 m_trans;

}; // TranslateJointAOP

}

#endif // _LIBSKELETON_TRANSLATE_JOINT_AOP_H_