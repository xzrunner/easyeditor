#include "Material.h"

namespace libshape
{

void Material::Translate(const d2d::Vector& offset)
{
	for (int i = 0, n = m_vertices.size(); i < n; ++i) {
		m_vertices[i] += offset;
	}
}

void Material::Refresh(const std::vector<d2d::Vector>& vertices)
{
	Build(vertices);
}

void Material::DebugDrawTris(const d2d::Matrix& mt) const
{
	std::vector<d2d::Vector> buf;
	for (size_t i = 0, n = m_vertices.size(); i < n; ++i)
	{
		buf.push_back(m_vertices[i]);
		if (buf.size() == 3)
		{
			d2d::PrimitiveDraw::drawPolyline(mt, buf, d2d::Colorf(0, 1, 0), true);
			buf.clear();
		}
	}
}


}