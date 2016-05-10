#ifndef _EASYMODELING_FIXTURE_H_
#define _EASYMODELING_FIXTURE_H_

#include <vector>

namespace ee { class Shape; class Rect; }

namespace emodeling
{

class Body;

class Fixture
{
public:
	Fixture();
	~Fixture();

	bool IsContain(const sm::vec2& pos) const;
	bool IsIntersect(const ee::Rect& rect) const;

	void Draw(const sm::mat4& mt, const s2::Color& cFace, 
		const s2::Color& cEdge) const;

private:
	void TransLocalToWorld(const std::vector<sm::vec2>& local, std::vector<sm::vec2>& world) const;

public:
	std::string m_name;

	Body* m_body;

	ee::Shape* m_shape;

	float m_density;

	float m_friction;
	float m_restitution;

	bool m_is_sensor;

	unsigned short m_category_bits;
	unsigned short m_mask_bits;
	short m_group_index;

}; // Fixture

}

#endif // _EASYMODELING_FIXTURE_H_
