//#include "SkeletonMesh.h"
//
//#include <ee/Symbol.h>
//#include <ee/Shape.h>
//
//#include <easyshape.h>
//
//#include <sprite2/PolylineShape.h>
//#include <gum/JsonSerializer.h>
//#include <gum/MeshIO.h>
//
//namespace emesh
//{
//
//SkeletonMesh::SkeletonMesh()
//{
//}
//
//SkeletonMesh::SkeletonMesh(const SkeletonMesh& mesh)
//	: s2::Mesh(mesh)
//	, Mesh(mesh)
//	, s2::SkeletonMesh(mesh)
//{
//}
//
//SkeletonMesh::SkeletonMesh(const ee::Symbol* base)
//	: s2::Mesh(base)
//	, Mesh(base)
//	, s2::SkeletonMesh(base)
//{
//}
//
//void SkeletonMesh::Load(const Json::Value& value)
//{
//	if (m_shape) {
//		delete m_shape;
//	}
//
//	std::vector<sm::vec2> vertices;
//	gum::JsonSerializer::Load(value["vertices"], vertices);
//	m_shape = new eshape::ChainShape(vertices, true);
//
//	gum::MeshIO::Load(value, m_skeleton);
//
//	RefreshTriangles();
//
//	pm::MeshTransform trans;
//	gum::MeshIO::Load(value, trans, *this);
//	trans.StoreToMesh(this);
//}
//
//void SkeletonMesh::Store(Json::Value& value) const
//{
//	value["type"] = GetTypeName();
//
//	if (!m_shape) {
//		return;
//	}
//
//	gum::JsonSerializer::Store(m_shape->GetVertices(), value["vertices"]);
//
//	gum::MeshIO::Store(value, m_skeleton);
//
//	pm::MeshTransform trans;
//	gum::MeshIO::Load(value, trans, *this);
//	trans.StoreToMesh(this);
//}
//
//void SkeletonMesh::OffsetUV(float dx, float dy)
//{
//}
//
//void SkeletonMesh::Refresh()
//{
//	RefreshTriangles();
//}
//
//bool SkeletonMesh::RemoveMesh(ee::Shape* shape)
//{
//	if (shape == dynamic_cast<ee::Shape*>(m_shape)) {
//		delete m_shape, m_shape = NULL;		
//		return true;
//	} else {
//		return false;
//	}
//}
//
//bool SkeletonMesh::InsertMesh(ee::Shape* shape)
//{
//	eshape::ChainShape* loop = dynamic_cast<eshape::ChainShape*>(shape);
//	if (m_shape) {
//		delete m_shape;
//	}
//	m_shape = new eshape::ChainShape(loop->GetVertices(), true);
//	return true;
//}
//
//bool SkeletonMesh::ClearMesh()
//{
//	if (m_shape) {
//		delete m_shape, m_shape = NULL;
//		return true;
//	} else {
//		return false;
//	}
//}
//
//void SkeletonMesh::Reset()
//{
//	RefreshTriangles();
//}
//
//void SkeletonMesh::Clear()
//{
//	ClearTriangles();
//}
//
//}