#ifndef _EASYSKELETON_JOINT_H_
#define _EASYSKELETON_JOINT_H_

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>
#include <SM_Vector.h>
#include <sprite2/JointPose.h>

#include <vector>

namespace ee { class Sprite; }

namespace eskeleton
{

class Joint : public cu::RefCountObj, private cu::Uncopyable
{
public:
	Joint(ee::Sprite* spr, const sm::vec2& offset);
	~Joint();

	void Draw() const;

	void UpdateToJoint();
	void UpdateToSkin();

	void Translate(const sm::vec2& offset);
	void Rotate(float angle);
	
	const sm::vec2& GetWorldPos() const { return m_world_pose.trans; }
	void SetWorldPos(const sm::vec2& pos, bool static_skin);

	const Joint* GetParent() const { return m_parent; }

	bool Connect(Joint* child);
	void Deconnect();

	void Clear();

	const s2::JointPose& GetWorldPose() const { return m_world_pose; }
	const s2::JointPose& GetLocalPose() const { return m_local_pose; }
	const s2::JointPose& GetSkinPose() const { return m_skin.pose; }

	void SetWorldPose(const s2::JointPose& pose) { m_world_pose = pose; }
	void SetLocalPose(const s2::JointPose& pose) { m_local_pose = pose; }

public:
	static const float RADIUS;

private:
	void UpdateChildren();

private:
	struct Skin : private cu::Uncopyable
	{
		ee::Sprite* spr;
		s2::JointPose pose;

		Skin(ee::Sprite* spr, const sm::vec2& pos);
		~Skin();

		void Update(const Joint* joint);

	}; // Skin

private:
	Joint* m_parent;
	std::vector<Joint*> m_children;	

	s2::JointPose m_world_pose;
	s2::JointPose m_local_pose;

	Skin m_skin;
	
}; // Joint

}

#endif // _EASYSKELETON_JOINT_H_