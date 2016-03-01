//#include "MultiColorMaterial.h"
//
//namespace eshape
//{
//
//MultiColorMaterial::MultiColorMaterial(const std::vector<ee::Vector>& vertices, 
//							 const ee::Colorf& color)
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
//void MultiColorMaterial::Draw(const ee::Matrix& mt) const
//{
//	ee::RVG::Triangles(mt, m_tris, m_color);
//}
//
//void MultiColorMaterial::Build(const std::vector<ee::Vector>& vertices)
//{
//	m_tris.clear();
//
//	std::vector<ee::Vector> bounding;
//	ee::Math2D::removeDuplicatePoints(vertices, bounding);
//
//	ee::Triangulation::normal(bounding, m_tris);
//}
//
//void MultiColorMaterial::Build(const std::vector<ee::Vector>& vertices, 
//						  const std::vector<ee::Vector>& segments)
//{
//	m_tris.clear();
//
//	std::vector<ee::Vector> bounding;
//	ee::Math2D::removeDuplicatePoints(vertices, bounding);
//
//	ee::Triangulation::lines(bounding, segments, m_tris);
//}
//
//}