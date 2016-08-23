#include "NetworkShape.h"

namespace s2
{

NetworkShape::NetworkShape()
	: PolylineShape()
{
	m_closed = true;
}

NetworkShape::NetworkShape(const NetworkShape& nw)
	: PolylineShape(nw)
	, m_inner_vertices(nw.m_inner_vertices)
{
}

NetworkShape::NetworkShape(const std::vector<sm::vec2>& vertices)
	: PolylineShape(vertices, true)
{
}

NetworkShape* NetworkShape::Clone() const
{
	return new NetworkShape(*this);
}

}