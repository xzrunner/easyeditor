#ifndef _EASYSKELETON_BONE_H_
#define _EASYSKELETON_BONE_H_

#include <ee/SpriteObserver.h>

#include <SM_Vector.h>
#include <cu/CU_RefCountObj.h>
#include <cu/uncopyable.h>

#include <vector>

namespace ee { class Sprite; }

namespace eskeleton
{

class Joint;

class Bone : public cu::RefCountObj, private cu::Uncopyable, public ee::SpriteObserver
{
public:
	Bone();
	Bone(const ee::SprPtr& skin);
	~Bone();

	/**
	 *  @interface
	 *    ee::SpriteObserver
	 */
	virtual void OnSetPosition(const ee::SprPtr& spr, const sm::vec2& pos);
	virtual void OnSetAngle(const ee::SprPtr& spr, float angle);
	virtual void OnSetScale(const ee::SprPtr& spr, const sm::vec2& scale);
	virtual void OnSetShear(const ee::SprPtr& spr, const sm::vec2& shear);
	virtual void OnSetOffset(const ee::SprPtr& spr, const sm::vec2& offset);

	void Update();
	void Draw() const;

	Joint* QueryJoint(const sm::vec2& pos) const;

	bool AutoAbsorb(const Bone* bone);

	void Translate(const sm::vec2& trans);
	void Rotate(float rot);
	void Scale(const sm::vec2& scale);

	void SetSkin(const ee::SprPtr& skin);
	void AddJoint(Joint* joint);
	void RemoveJoint(Joint* joint);

	const std::vector<Joint*>& GetAllJoints() const { return m_joints; }

private:
	void InitJoints();

	Joint* GetCenterJoint();

private:
	ee::SprPtr m_skin;

	std::vector<Joint*> m_joints;

	bool m_is_updating;

}; // Bone

}

#endif // _EASYSKELETON_BONE_H_