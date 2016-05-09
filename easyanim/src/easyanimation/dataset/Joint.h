#ifndef _EASYANIM_JOINT_H_
#define _EASYANIM_JOINT_H_

#include <set>

namespace ee { class Sprite; }

namespace eanim
{

class Joint
{
public:
	Joint(ee::Sprite* sprite);
	Joint(ee::Sprite* sprite, const sm::vec2& pos);
	~Joint();

	void Draw() const;

	bool Contain(const sm::vec2& pos) const;
	bool Intersect(const sm::vec2& pos) const;

	void SetPosition(const sm::vec2& pos);

	sm::vec2 GetWorldPos() const;
	sm::vec2 GetRelativePos(const sm::vec2& pos) const;

	bool Connect(Joint* joint);
	void Deconnect();

private:
	void CreateId();

	sm::vec2 GetRelativePos() const;

private:
	int m_id;

	ee::Sprite* m_sprite;

	sm::vec2 m_relative_pos;
	float m_relative_angle;

	Joint* m_parent;
	std::set<Joint*> m_children;

	friend class SkeletonData;
	friend class FileIO;

}; // Joint

}

#endif // _EASYANIM_JOINT_H_