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
	Bone(ee::Sprite* skin);
	~Bone();

	void Update();
	void Draw() const;

	Joint* QueryJoint(const sm::vec2& pos) const;

	bool AutoAbsorb(const Bone* bone);

	void Translate(const sm::vec2& offset);

private:
	ee::Sprite* m_skin;

	std::vector<Joint*> m_joints;

}; // Bone

}

#endif // _EASYSKELETON_BONE_H_