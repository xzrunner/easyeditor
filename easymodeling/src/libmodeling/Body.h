#ifndef _EASYMODELING_BODY_H_
#define _EASYMODELING_BODY_H_

#include <SM_Matrix.h>
#include <SM_Rect.h>
#include <CU_RefCountObj.h>

#include <vector>

namespace ee { class Rect; class Sprite; }
namespace s2 { class Color; }

namespace emodeling
{

class Fixture;

class Body : public cu::RefCountObj
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

	bool IsContain(const sm::vec2& pos) const;
	bool IsIntersect(const sm::rect& rect) const;

	void Draw(const sm::mat4& mt, const s2::Color& cFace, 
		const s2::Color& cEdge) const;

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
