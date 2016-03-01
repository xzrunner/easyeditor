#include "ColorMaterial.h"

#include <ee/ColorTrans.h>
#include <ee/EE_RVG.h>
#include <ee/Math2D.h>
#include <ee/Triangulation.h>

namespace eshape
{

ColorMaterial::ColorMaterial(const std::vector<ee::Vector>& vertices, 
							 const ee::Colorf& color)
	: m_color(color)
{
	BuildBegin(vertices);
	BuildEnd();
}

Json::Value ColorMaterial::Store(const std::string& dirpath) const
{
	Json::Value val;
	val["type"] = "color";
	val["color"] = m_color.Pack();
	return val;
}

void ColorMaterial::Draw(const ee::Matrix& mt, const ee::ColorTrans& color) const
{
	ee::Colorf c = ee::col_mul(m_color, color.multi);
	ee::RVG::Triangles(mt, m_tris, c);

	//ee::ShaderMgr::Instance()->RVG();
	//ee::Colorf c = ee::cMul(m_color, color.multi);
	//ee::PrimitiveDrawRVG::SetColor(ee::trans_color2int(c, ee::PT_ABGR));
	//ee::PrimitiveDrawRVG::Triangles(mt, m_tris);
}

void ColorMaterial::BuildEnd()
{
	m_tris.clear();

	std::vector<ee::Vector> outline;
	ee::Math2D::RemoveDuplicatePoints(m_outline, outline);

	if (!m_segments.empty()) {
		ee::Triangulation::Lines(m_outline, m_segments, m_tris);
	} else if (!m_holes.empty()) {
		ee::Triangulation::Holes(m_outline, m_holes, m_tris);
	} else {
		ee::Triangulation::Normal(m_outline, m_tris);
	}
}

}