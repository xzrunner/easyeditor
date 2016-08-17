#include "Body.h"
#include "Fixture.h"

#include <ee/StringHelper.h>
#include <ee/std_functor.h>

namespace emodeling
{

Body::Body()
	: m_type(e_static)
	, m_linear_damping(0.0f)
	, m_angular_damping(0.0f)
	, m_allow_sleep(true)
	, m_bullet(false)
	, m_active(true)
	, m_gravity_scale(1.0f)
	, m_spr(NULL)
{
	static int count = 0;
	m_name = std::string("body") + ee::StringHelper::ToString(count++);
}

Body::~Body()
{
	for_each(m_fixtures.begin(), m_fixtures.end(), ee::DeletePointerFunctor<Fixture>());
	m_fixtures.clear();
}

bool Body::IsContain(const sm::vec2& pos) const
{
	for (size_t i = 0, n = m_fixtures.size(); i < n; ++i)
		if (m_fixtures[i]->IsContain(pos))
			return true;
	return false;
}

bool Body::IsIntersect(const sm::rect& rect) const
{
	for (size_t i = 0, n = m_fixtures.size(); i < n; ++i)
		if (m_fixtures[i]->IsIntersect(rect))
			return true;
	return false;
}

void Body::Draw(const sm::mat4& mt, const s2::Color& cFace, const s2::Color& cEdge) const
{
	for (size_t i = 0, n = m_fixtures.size(); i < n; ++i) {
		m_fixtures[i]->Draw(mt, cFace, cEdge);
	}
}

}