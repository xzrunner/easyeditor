//#include "MultiColorMaterial.h"
//
//namespace eshape
//{
//
//MultiColorMaterial::MultiColorMaterial(const std::vector<sm::vec2>& vertices, 
//							 const s2::Color& color)
//{
//	Build(vertices);
//}
//
//Json::Value MultiColorMaterial::Store(const std::string& dirpath) const
//{
//	Json::Value val;
//	val["type"] = "multi-color";
//	val[""]
//	return val;
//}
//
//void MultiColorMaterial::Draw(const sm::mat4& mt) const
//{
//	s2::RVG::Triangles(mt, m_tris, m_color);
//}
//
//void MultiColorMaterial::Build(const std::vector<sm::vec2>& vertices)
//{
//	m_tris.clear();
//
//	std::vector<sm::vec2> bounding;
//	ee::Math2D::removeDuplicatePoints(vertices, bounding);
//
//	ee::Triangulation::normal(bounding, m_tris);
//}
//
//void MultiColorMaterial::Build(const std::vector<sm::vec2>& vertices, 
//						  const std::vector<sm::vec2>& segments)
//{
//	m_tris.clear();
//
//	std::vector<sm::vec2> bounding;
//	ee::Math2D::removeDuplicatePoints(vertices, bounding);
//
//	sm::triangulate_lines(bounding, segments, m_tris);
//}
//
//}