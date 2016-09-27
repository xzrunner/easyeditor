#ifndef _EASYSKELETON_JOINT_H_
#define _EASYSKELETON_JOINT_H_

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>
#include <SM_Vector.h>

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

	const sm::vec2& GetWorldPos() const { return m_world_pos; }
	void SetWorldPos(const sm::vec2& pos, bool static_skin);

	bool Connect(Joint* child);
	void Deconnect();

public:
	static const float RADIUS;

private:
	void UpdateChildren();

	static void CalWorld(
		const sm::vec2& src_world_pos, float src_world_angle, 
		const sm::vec2& local_pos, float local_angle, 
		sm::vec2& dst_world_pos, float& dst_world_angle);

private:
	struct Pose
	{
		sm::vec2 pos;
		float angle;
		float scale;

		Pose() : angle(0), scale(1) {}
	};

	struct Skin
	{
		ee::Sprite* spr;
		sm::vec2 pos;
		float angle;

		Skin(ee::Sprite* spr, const sm::vec2& pos);
		~Skin();

		void Update(const Joint* joint);
	};

private:
	Joint* m_parent;
	std::vector<Joint*> m_children;	

	sm::vec2 m_world_pos;
	float m_world_angle;

	Pose m_pose;

	Skin m_skin;
	
}; // Joint

}

#endif // _EASYSKELETON_JOINT_H_