//#include "MultiColorMaterial.h"
//
//namespace libshape
//{
//
//MultiColorMaterial::MultiColorMaterial(const std::vector<d2d::Vector>& vertices, 
//							 const d2d::Colorf& color)
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
//void MultiColorMaterial::Draw(const d2d::Matrix& mt) const
//{
//	d2d::PrimitiveDraw::drawTriangles(mt, m_tris, m_color);
//}
//
//void MultiColorMaterial::Build(const std::vector<d2d::Vector>& vertices)
//{
//	m_tris.clear();
//
//	std::vector<d2d::Vector> bounding;
//	d2d::Math::removeDuplicatePoints(vertices, bounding);
//
//	d2d::Triangulation::normal(bounding, m_tris);
//}
//
//void MultiColorMaterial::Build(const std::vector<d2d::Vector>& vertices, 
//						  const std::vector<d2d::Vector>& segments)
//{
//	m_tris.clear();
//
//	std::vector<d2d::Vector> bounding;
//	d2d::Math::removeDuplicatePoints(vertices, bounding);
//
//	d2d::Triangulation::lines(bounding, segments, m_tris);
//}
//
//}