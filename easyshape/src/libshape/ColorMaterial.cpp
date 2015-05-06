#include "ColorMaterial.h"

namespace libshape
{

ColorMaterial::ColorMaterial(const std::vector<d2d::Vector>& vertices, 
							 const d2d::Colorf& color)
	: m_color(color)
{
	BuildBegin(vertices);
	BuildEnd();
}

Json::Value ColorMaterial::Store(const std::string& dirpath) const
{
	Json::Value val;
	val["type"] = "color";
	val["color"] = m_color.pack();
	return val;
}

void ColorMaterial::Draw(const d2d::Matrix& mt) const
{
	d2d::PrimitiveDraw::drawTriangles(mt, m_tris, m_color);
}

void ColorMaterial::BuildEnd()
{
	m_tris.clear();

	std::vector<d2d::Vector> outline;
	d2d::Math::removeDuplicatePoints(m_outline, outline);

	if (!m_segments.empty()) {
		d2d::Triangulation::lines(m_outline, m_segments, m_tris);
	} else if (!m_holes.empty()) {
		d2d::Triangulation::Holes(m_outline, m_holes, m_tris);
	} else {
		d2d::Triangulation::normal(m_outline, m_tris);
	}
}

}