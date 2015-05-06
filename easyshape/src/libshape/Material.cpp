#include "Material.h"

namespace libshape
{

void Material::Translate(const d2d::Vector& offset)
{
	for (int i = 0, n = m_tris.size(); i < n; ++i) {
		m_tris[i] += offset;
	}
}

void Material::Refresh(const std::vector<d2d::Vector>& vertices)
{
	BuildBegin(vertices);
	BuildEnd();
}

void Material::DebugDrawTris(const d2d::Matrix& mt) const
{
	std::vector<d2d::Vector> buf;
	for (size_t i = 0, n = m_tris.size(); i < n; ++i)
	{
		buf.push_back(m_tris[i]);
		if (buf.size() == 3)
		{
			d2d::PrimitiveDraw::drawPolyline(mt, buf, d2d::Colorf(0, 1, 0), true);
			buf.clear();
		}
	}
}

void Material::BuildBegin(const std::vector<d2d::Vector>& outline)
{
	m_outline = outline;
	m_segments.clear();
	m_holes.clear();
}

void Material::BuildSetSegments(const std::vector<d2d::Vector>& segs) 
{ 
	m_segments = segs;
}

void Material::BuildSetHoles(const std::vector<std::vector<d2d::Vector> >& holes) 
{ 
	m_holes = holes; 
}

}