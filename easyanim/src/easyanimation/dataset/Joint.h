#ifndef _EASYANIM_JOINT_H_
#define _EASYANIM_JOINT_H_

#include <SM_Vector.h>

#include <set>

namespace ee { class Sprite; }

namespace eanim
{

class Joint
{
public:
	Joint(ee::Sprite* spr);
	Joint(ee::Sprite* spr, const sm::vec2& pos);
	~Joint();

	void Draw() const;

	bool Contain(const sm::vec2& pos) const;
	bool Intersect(const sm::vec2& pos) const;

	void SetPosition(const sm::vec2& pos);

	sm::vec2 GetWorldPos() const;
	sm::vec2 GetRelativePos(const sm::vec2& pos) const;

	bool Connect(Joint* joint);
	void Deconnect();

	// todo: DefaultFileLoader
	void SetID(int id) { m_id = id; }
	void SetRelativePos(const sm::vec2& pos) { m_relative_pos = pos; }
	void SetParent(Joint* parent) { m_parent = parent; }
	void AddChild(Joint* child) { m_children.insert(child); }

	// todo: FileSaver
	int GetID() const { return m_id; }
	const sm::vec2& GetCurrRelativePos() const { return m_relative_pos; }
	const Joint* GetParent() const { return m_parent; }
	const std::set<Joint*>& GetChildren() const { return m_children; }

private:
	void CreateId();

	sm::vec2 GetRelativePos() const;

private:
	int m_id;

	ee::Sprite* m_spr;

	sm::vec2 m_relative_pos;
	float m_relative_angle;

	Joint* m_parent;
	std::set<Joint*> m_children;

	friend class SkeletonData;

}; // Joint

}

#endif // _EASYANIM_JOINT_H_