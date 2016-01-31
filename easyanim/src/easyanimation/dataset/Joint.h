#ifndef _EASYANIM_JOINT_H_
#define _EASYANIM_JOINT_H_



namespace eanim
{

class Joint
{
public:
	Joint(ee::Sprite* sprite);
	Joint(ee::Sprite* sprite, const ee::Vector& pos);
	~Joint();

	void Draw() const;

	bool Contain(const ee::Vector& pos) const;
	bool Intersect(const ee::Vector& pos) const;

	void SetPosition(const ee::Vector& pos);

	ee::Vector GetWorldPos() const;
	ee::Vector GetRelativePos(const ee::Vector& pos) const;

	bool Connect(Joint* joint);
	void Deconnect();

private:
	void CreateId();

private:
	int m_id;

	ee::Sprite* m_sprite;

	ee::Vector m_relative;
	float m_relativeAngle;

	Joint* m_parent;
	std::set<Joint*> m_children;

	friend class SkeletonData;
	friend class FileIO;

}; // Joint

}

#endif // _EASYANIM_JOINT_H_