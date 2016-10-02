#include "Skeleton.h"

#include <ee/Symbol.h>
#include <ee/Shape.h>

#include <easyshape.h>

#include <sprite2/PolylineShape.h>
#include <sprite2/MeshTransform.h>
#include <gum/JsonSerializer.h>
#include <gum/MeshIO.h>

namespace emesh
{

Skeleton::Skeleton()
{
}

Skeleton::Skeleton(const Skeleton& mesh)
	: s2::Mesh(mesh)
	, Mesh(mesh)
	, s2::SkeletonMesh(mesh)
{
}

Skeleton::Skeleton(const ee::Symbol* base)
	: s2::Mesh(base)
	, Mesh(base)
	, s2::SkeletonMesh(base)
{
}

void Skeleton::Load(const Json::Value& value)
{
	if (m_shape) {
		delete m_shape;
	}

	std::vector<sm::vec2> vertices;
	gum::JsonSerializer::Load(value["vertices"], vertices);
	m_shape = new eshape::ChainShape(vertices, true);

	gum::MeshIO::Load(value, m_skeleton);

	RefreshTriangles();

	s2::MeshTransform trans;
	gum::MeshIO::Load(value, trans);
	trans.StoreToMesh(this);
}

void Skeleton::Store(Json::Value& value) const
{
	value["type"] = GetTypeName();

	if (!m_shape) {
		return;
	}

	gum::JsonSerializer::Store(m_shape->GetVertices(), value["vertices"]);

	gum::MeshIO::Store(value, m_skeleton);

	s2::MeshTransform trans;
	gum::MeshIO::Load(value, trans);
	trans.StoreToMesh(this);
}

void Skeleton::OffsetUV(float dx, float dy)
{
}

void Skeleton::Refresh()
{
	RefreshTriangles();
}

void Skeleton::TraverseMesh(ee::Visitor<ee::Shape>& visitor) const
{
	if (m_shape) {
		bool has_next;
		visitor.Visit(dynamic_cast<ee::Shape*>(m_shape), has_next);
	}
}

bool Skeleton::RemoveMesh(ee::Shape* shape)
{
	if (shape == dynamic_cast<ee::Shape*>(m_shape)) {
		delete m_shape, m_shape = NULL;		
		return true;
	} else {
		return false;
	}
}

bool Skeleton::InsertMesh(ee::Shape* shape)
{
	eshape::ChainShape* loop = dynamic_cast<eshape::ChainShape*>(shape);
	if (m_shape) {
		delete m_shape;
	}
	m_shape = new eshape::ChainShape(loop->GetVertices(), true);
	return true;
}

bool Skeleton::ClearMesh()
{
	if (m_shape) {
		delete m_shape, m_shape = NULL;
		return true;
	} else {
		return false;
	}
}

void Skeleton::Reset()
{
	RefreshTriangles();
}

void Skeleton::Clear()
{
	ClearTriangles();
}

}