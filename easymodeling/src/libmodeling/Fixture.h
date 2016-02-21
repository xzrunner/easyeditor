#ifndef _EASYMODELING_FIXTURE_H_
#define _EASYMODELING_FIXTURE_H_

#include <ee/Color.h>

#include <vector>

namespace ee { class Shape; class Vector; class Rect; class Matrix; }

namespace emodeling
{

class Body;

class Fixture
{
public:
	Fixture();
	~Fixture();

	bool IsContain(const ee::Vector& pos) const;
	bool IsIntersect(const ee::Rect& rect) const;

	void Draw(const ee::Matrix& mt, const ee::Colorf& cFace, 
		const ee::Colorf& cEdge) const;

private:
	void TransLocalToWorld(const std::vector<ee::Vector>& local, std::vector<ee::Vector>& world) const;

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
