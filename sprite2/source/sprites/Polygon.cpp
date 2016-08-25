#include "Polygon.h"
#include "RenderParams.h"
#include "S2_RVG.h"

#include <SM_Process.h>

namespace s2
{

void Polygon::Clear()
{
	m_tris.clear();
	m_outline.clear();
	m_segments.clear();
	m_holes.clear();
}

void Polygon::DebugDraw(const sm::mat4& mt) const
{
	std::vector<sm::vec2> buf;
	for (size_t i = 0, n = m_tris.size(); i < n; ++i)
	{
		buf.push_back(m_tris[i]);
		if (buf.size() == 3)
		{
			std::vector<sm::vec2> vertices;
			sm::trans_vertices(mt, buf, vertices);
			RVG::SetColor(Color(0, 255, 0));
			RVG::Polyline(vertices, true);
			buf.clear();
		}
	}
}

}