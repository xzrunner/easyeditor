#ifndef _EASYSKELETON_BONE_H_
#define _EASYSKELETON_BONE_H_

#include <ee/SpriteObserver.h>

#include <SM_Vector.h>
#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>

#include <vector>

namespace ee { class Sprite; }

namespace eskeleton
{

class Joint;

class Bone : public cu::RefCountObj, private cu::Uncopyable, public ee::SpriteObserver
{
public:
	Bone();
	Bone(ee::Sprite* skin);
	~Bone();

	/**
	 *  @interface
	 *    ee::SpriteObserver
	 */
	virtual void OnSetPosition(ee::Sprite* spr, const sm::vec2& pos);
	virtual void OnSetAngle(ee::Sprite* spr, float angle);
	virtual void OnSetScale(ee::Sprite* spr, const sm::vec2& scale);
	virtual void OnSetShear(ee::Sprite* spr, const sm::vec2& shear);
	virtual void OnSetOffset(ee::Sprite* spr, const sm::vec2& offset);

	void Update();
	void Draw() const;

	Joint* QueryJoint(const sm::vec2& pos) const;

	bool AutoAbsorb(const Bone* bone);

	void Translate(const sm::vec2& trans);
	void Rotate(float rot);
	void Scale(const sm::vec2& scale);

	void SetSkin(ee::Sprite* skin);
	void AddJoint(Joint* joint);
	void RemoveJoint(Joint* joint);

	const std::vector<Joint*>& GetAllJoints() const { return m_joints; }

private:
	void InitJoints();

	Joint* GetCenterJoint();

private:
	ee::Sprite* m_skin;

	std::vector<Joint*> m_joints;

	bool m_is_updating;

}; // Bone

}

#endif // _EASYSKELETON_BONE_H_