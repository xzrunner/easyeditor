#ifndef _EASYMODELING_BODY_H_
#define _EASYMODELING_BODY_H_

#include <ee/Color.h>
#include <ee/Object.h>

#include <vector>

namespace ee { class Vector; class Rect; class Sprite; }

namespace emodeling
{

class Fixture;

class Body : public ee::Object
{
public:
	enum Type
	{
		e_static = 0,
		e_kinematic,
		e_dynamic
	};

public:
	Body();
	~Body();

	bool IsContain(const ee::Vector& pos) const;
	bool IsIntersect(const ee::Rect& rect) const;

	void Draw(const sm::mat4& mt, const ee::Colorf& cFace, 
		const ee::Colorf& cEdge) const;

public:
	std::string m_name;

	Type m_type;

	float m_linear_damping;

	float m_angular_damping;

	bool m_allow_sleep;

	bool m_bullet;

	bool m_active;

	float m_gravity_scale;

	ee::Sprite* m_sprite;

	std::vector<Fixture*> m_fixtures;

}; // Body

}

#endif // _EASYMODELING_BODY_H_
