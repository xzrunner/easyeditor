#ifndef _EASYSKELETON_JOINT_H_
#define _EASYSKELETON_JOINT_H_

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>
#include <SM_Vector.h>
#include <sprite2/JointCoords.h>

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

	void Translate(const sm::vec2& trans);
	void Rotate(float rot);
	
	const sm::vec2& GetWorldPos() const { return m_world.pos; }
	void SetWorldPos(const sm::vec2& pos, bool static_skin);

	const Joint* GetParent() const { return m_parent; }

	bool Connect(Joint* child);
	void Deconnect();

	void Clear();

	const s2::WorldPose& GetWorldPose() const { return m_world; }
	const s2::LocalPose& GetLocalPose() const { return m_local; }
	const sm::vec2& GetSkinPose() const { return m_skin.offset; }

	void SetWorldPose(const s2::WorldPose& pose) { m_world = pose; }
	void SetLocalPose(const s2::LocalPose& pose) { m_local = pose; }

public:
	static const float RADIUS;

private:
	void UpdateChildren();

private:
	struct Skin : private cu::Uncopyable
	{
		ee::Sprite* spr;
		sm::vec2 offset;

		Skin(ee::Sprite* spr, const sm::vec2& offset);
		~Skin();

		void Update(const Joint* joint);

	}; // Skin

private:
	Joint* m_parent;
	std::vector<Joint*> m_children;	
	
	s2::WorldPose m_world;
	s2::LocalPose m_local;

	Skin m_skin;
	
}; // Joint

}

#endif // _EASYSKELETON_JOINT_H_