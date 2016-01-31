
#include "Body.h"
#include "Fixture.h"

using namespace libmodeling;

Body::Body()
	: type(e_static)
	, linearDamping(0.0f)
	, angularDamping(0.0f)
	, allowSleep(true)
	, bullet(false)
	, active(true)
	, gravityScale(1.0f)
	, sprite(NULL)
{
	static int count = 0;
	name = wxT("body") + wxString::FromDouble(count++);
}

Body::~Body()
{
	for_each(fixtures.begin(), fixtures.end(), DeletePointerFunctor<Fixture>());
	fixtures.clear();
}

bool Body::isContain(const ee::Vector& pos) const
{
	for (size_t i = 0, n = fixtures.size(); i < n; ++i)
		if (fixtures[i]->isContain(pos))
			return true;
	return false;
}

bool Body::isIntersect(const ee::Rect& rect) const
{
	for (size_t i = 0, n = fixtures.size(); i < n; ++i)
		if (fixtures[i]->isIntersect(rect))
			return true;
	return false;
}

void Body::draw(const ee::Matrix& mt, const ee::Colorf& cFace, const ee::Colorf& cEdge) const
{
	for (size_t i = 0, n = fixtures.size(); i < n; ++i) {
		fixtures[i]->draw(mt, cFace, cEdge);
	}
}
