#ifndef _EASYANIM_JOINT_H_
#define _EASYANIM_JOINT_H_

#include <drag2d.h>

namespace eanim
{

class Joint
{
public:
	Joint(d2d::Sprite* sprite);
	Joint(d2d::Sprite* sprite, const d2d::Vector& pos);
	~Joint();

	void Draw() const;

	bool Contain(const d2d::Vector& pos) const;
	bool Intersect(const d2d::Vector& pos) const;

	void SetPosition(const d2d::Vector& pos);

	d2d::Vector GetWorldPos() const;
	d2d::Vector GetRelativePos(const d2d::Vector& pos) const;

	bool Connect(Joint* joint);
	void Deconnect();

private:
	void CreateId();

private:
	int m_id;

	d2d::Sprite* m_sprite;

	d2d::Vector m_relative;
	float m_relativeAngle;

	Joint* m_parent;
	std::set<Joint*> m_children;

	friend class SkeletonData;
	friend class FileIO;

}; // Joint

}

#endif // _EASYANIM_JOINT_H_