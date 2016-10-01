#ifndef _EASYSKELETON_BONE_H_
#define _EASYSKELETON_BONE_H_

#include <SM_Vector.h>
#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>

#include <vector>

namespace ee { class Sprite; }

namespace eskeleton
{

class Joint;

class Bone : public cu::RefCountObj, private cu::Uncopyable
{
public:
	Bone();
	Bone(ee::Sprite* skin);
	~Bone();

	void Update();
	void Draw() const;

	Joint* QueryJoint(const sm::vec2& pos) const;

	bool AutoAbsorb(const Bone* bone);

	void Translate(const sm::vec2& offset);
	bool Rotate(float angle);

	void SetSkin(ee::Sprite* skin);
	void AddJoint(Joint* joint);
	void RemoveJoint(Joint* joint);

	const std::vector<Joint*>& GetAllJoints() const { return m_joints; }

private:
	void InitJoints();

private:
	ee::Sprite* m_skin;

	std::vector<Joint*> m_joints;

}; // Bone

}

#endif // _EASYSKELETON_BONE_H_