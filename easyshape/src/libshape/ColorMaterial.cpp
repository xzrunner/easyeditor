#include "ColorMaterial.h"

namespace libshape
{

ColorMaterial::ColorMaterial(const std::vector<d2d::Vector>& vertices, 
							 const d2d::Colorf& color)
	: m_color(color)
{
	Build(vertices);
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
	d2d::PrimitiveDraw::drawTriangles(mt, m_vertices, m_color);
}

void ColorMaterial::Build(const std::vector<d2d::Vector>& vertices)
{
	m_vertices.clear();

	std::vector<d2d::Vector> bounding;
	d2d::Math::removeDuplicatePoints(vertices, bounding);

	d2d::Triangulation::normal(bounding, m_vertices);
}

void ColorMaterial::Build(const std::vector<d2d::Vector>& vertices, 
						  const std::vector<d2d::Vector>& segments)
{
	m_vertices.clear();

	std::vector<d2d::Vector> bounding;
	d2d::Math::removeDuplicatePoints(vertices, bounding);

	d2d::Triangulation::lines(bounding, segments, m_vertices);
}

}