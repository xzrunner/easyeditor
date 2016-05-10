#include "Material.h"

#include <ee/EE_RVG.h>
#include <ee/Math2D.h>

namespace eshape
{

void Material::Translate(const sm::vec2& offset)
{
	for (int i = 0, n = m_tris.size(); i < n; ++i) {
		m_tris[i] += offset;
	}
}

void Material::BuildBegin(const std::vector<sm::vec2>& outline)
{
	m_outline = outline;
	m_segments.clear();
	m_holes.clear();
}

void Material::BuildSetSegments(const std::vector<sm::vec2>& segs) 
{ 
	m_segments = segs;
}

void Material::BuildSetHoles(const std::vector<std::vector<sm::vec2> >& holes) 
{ 
	m_holes = holes; 
}

void Material::Refresh(const std::vector<sm::vec2>& vertices)
{
	BuildBegin(vertices);
	BuildEnd();
}

void Material::DebugDrawTris(const sm::mat4& mt) const
{
	std::vector<sm::vec2> buf;
	for (size_t i = 0, n = m_tris.size(); i < n; ++i)
	{
		buf.push_back(m_tris[i]);
		if (buf.size() == 3)
		{
			std::vector<sm::vec2> vertices;
			ee::Math2D::TransVertices(mt, buf, vertices);
			ee::RVG::Color(s2::Color(0, 255, 0));
			ee::RVG::Polyline(vertices, true);
			buf.clear();
		}
	}
}

}