#include "Material.h"

#include <ee/EE_RVG.h>
#include <ee/Math2D.h>
#include <ee/Color.h>

namespace eshape
{

void Material::Translate(const ee::Vector& offset)
{
	for (int i = 0, n = m_tris.size(); i < n; ++i) {
		m_tris[i] += offset;
	}
}

void Material::BuildBegin(const std::vector<ee::Vector>& outline)
{
	m_outline = outline;
	m_segments.clear();
	m_holes.clear();
}

void Material::BuildSetSegments(const std::vector<ee::Vector>& segs) 
{ 
	m_segments = segs;
}

void Material::BuildSetHoles(const std::vector<std::vector<ee::Vector> >& holes) 
{ 
	m_holes = holes; 
}

void Material::Refresh(const std::vector<ee::Vector>& vertices)
{
	BuildBegin(vertices);
	BuildEnd();
}

void Material::DebugDrawTris(const ee::Matrix& mt) const
{
	std::vector<ee::Vector> buf;
	for (size_t i = 0, n = m_tris.size(); i < n; ++i)
	{
		buf.push_back(m_tris[i]);
		if (buf.size() == 3)
		{
			std::vector<ee::Vector> vertices;
			ee::Math2D::TransVertices(mt, buf, vertices);
			ee::RVG::Color(ee::Colorf(0, 1, 0));
			ee::RVG::Polyline(vertices, true);
			buf.clear();
		}
	}
}

}