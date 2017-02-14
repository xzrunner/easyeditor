#include "PointsMesh.h"
#include "color_config.h"

#include <easyshape.h>

#include <SM_Test.h>
#include <SM_Calc.h>
#include <polymesh/PointsMesh.h>
#include <polymesh/MeshTransform.h>
#include <sprite2/S2_RVG.h>
#include <gum/JsonSerializer.h>
#include <gum/MeshIO.h>

#include <assert.h>

namespace emesh
{

PointsMesh::PointsMesh()
	: Mesh()
{
	m_closed = true;
}

PointsMesh::PointsMesh(const ee::Symbol* base)
	: Mesh(base)
{
	m_closed = true;
}

void PointsMesh::Load(const Json::Value& value)
{
	if (m_mesh) {
		delete m_mesh;
	}

	std::vector<sm::vec2> outline, points;
	gum::JsonSerializer::Load(value["shape"]["outline"], outline);
	if (value["shape"].isMember("inner")) {
		gum::JsonSerializer::Load(value["shape"]["inner"], points);
	} else {
		gum::JsonSerializer::Load(value["shape"]["points"], points);
	}

	m_mesh = new pm::PointsMesh(outline, points, GetWidth(), GetHeight());

	pm::MeshTransform trans;
	gum::MeshIO::Load(value, trans, *this);
	m_mesh->LoadFromTransform(trans);

	m_vertices = outline;
	UpdateBounding();
}

void PointsMesh::Store(Json::Value& value) const
{
	if (!m_mesh) {
		return;
	}

	value["type"] = "points";

	assert(m_mesh->Type() == pm::MESH_POINTS);
	const pm::PointsMesh* points_mesh = static_cast<const pm::PointsMesh*>(m_mesh);
	gum::JsonSerializer::Store(points_mesh->GetOutline(), value["shape"]["outline"]);
	gum::JsonSerializer::Store(points_mesh->GetPoints(), value["shape"]["points"]);

	pm::MeshTransform trans;
	m_mesh->StoreToTransform(trans);
	gum::MeshIO::Store(value, trans, *this);
}

void PointsMesh::Refresh()
{
	// todo: rebuild tris
}

void PointsMesh::TraverseMesh(ee::Visitor<ee::Shape>& visitor) const
{
	const ee::Shape* shape = dynamic_cast<const ee::Shape*>(this);
	bool has_next;
	visitor.Visit(const_cast<ee::Shape*>(shape), has_next);
}

bool PointsMesh::RemoveMesh(ee::Shape* mesh)
{
	return false;
}

bool PointsMesh::InsertMesh(ee::Shape* mesh)
{
	eshape::ChainShape* chain = dynamic_cast<eshape::ChainShape*>(mesh);
	assert(chain);

	m_vertices = chain->GetVertices();
	if (!m_vertices.empty()) {
		m_vertices.pop_back();
	}
	UpdateBounding();	
	
	std::vector<sm::vec2> points;
	if (m_mesh)
	{
		assert(m_mesh->Type() == pm::MESH_POINTS);
		pm::PointsMesh* points_mesh = static_cast<pm::PointsMesh*>(m_mesh);	
		points_mesh->Build(m_vertices, points);
	}
	else
	{
		m_mesh = new pm::PointsMesh(m_vertices, points, GetWidth(), GetHeight());
	}

	return true;
}

void PointsMesh::Reset()
{
	if (!m_mesh) {
		return;
	}

	assert(m_mesh->Type() == pm::MESH_POINTS);
	pm::PointsMesh* points_mesh = static_cast<pm::PointsMesh*>(m_mesh);
	points_mesh->Build(points_mesh->GetOutline(), points_mesh->GetPoints());
}

void PointsMesh::Clear()
{
	if (!m_mesh) {
		return;
	}

	assert(m_mesh->Type() == pm::MESH_POINTS);
	pm::PointsMesh* points_mesh = static_cast<pm::PointsMesh*>(m_mesh);
	points_mesh->Clear();
}

//void PointsMesh::LoadFromFile(const Json::Value& value, const std::string& dir)
//{
// 	const Json::Value& shape_val = value["shape"];
// 	std::vector<sm::vec2> vertices;
// 	gum::JsonSerializer::Load(shape_val["outline"], vertices);
// 	m_vertices = vertices;
// 	UpdateBounding();
// 	gum::JsonSerializer::Load(shape_val["inner"], m_points);
//}
//
//void PointsMesh::StoreToFile(Json::Value& value, const std::string& dir) const
//{
// 	Json::Value& shape_val = value["shape"];
// 	gum::JsonSerializer::Store(m_vertices, shape_val["outline"]);
// 	gum::JsonSerializer::Store(m_points, shape_val["inner"]);
//}

void PointsMesh::AddVertex(int index, const sm::vec2& pos)
{
	eshape::PolylineEditor::AddVertex(m_vertices, m_bounding, index, pos);
	UpdateMeshFromShape();
}

void PointsMesh::RemoveVertex(const sm::vec2& pos)
{
	eshape::PolylineEditor::RemoveVertex(m_vertices, m_bounding, pos);
	UpdateMeshFromShape();
}

void PointsMesh::ChangeVertex(const sm::vec2& from, const sm::vec2& to)
{
	eshape::PolylineEditor::ChangeVertex(m_vertices, m_bounding, from, to);
	UpdateMeshFromShape();
}

void PointsMesh::SetVertices(const std::vector<sm::vec2>& vertices)
{
	m_vertices = vertices;
	UpdateMeshFromShape();
	UpdateBounding();
}

int PointsMesh::PointQueryVertex(const sm::vec2& p) const
{
	std::vector<sm::vec2> vertices, texcoords;
	std::vector<int> triangles;
	m_mesh->Dump(vertices, texcoords, triangles);
	
	int ret = -1;
	float nearest = FLT_MAX;
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		float dis = sm::dis_pos_to_pos(vertices[i], p);
		if (dis < GetNodeRadius() && dis < nearest) {
			nearest = dis;
			ret = i;
		}
	}

	return ret;
}

void PointsMesh::RectQueryVertices(const sm::rect& r, std::vector<int>& out_vertices) const
{
	std::vector<sm::vec2> vertices, texcoords;
	std::vector<int> triangles;
	m_mesh->Dump(vertices, texcoords, triangles);

	for (int i = 0, n = vertices.size(); i < n; ++i) {
		if (sm::is_point_in_rect(vertices[i], r)) {
			out_vertices.push_back(i);
		}
	}
}

int PointsMesh::QueryInnerPos(const sm::vec2& pos) const
{
	if (!m_mesh) {
		return -1;
	}

	assert(m_mesh->Type() == pm::MESH_POINTS);
	pm::PointsMesh* points_mesh = static_cast<pm::PointsMesh*>(m_mesh);	
	const std::vector<sm::vec2>& points = points_mesh->GetPoints();
	for (int i = 0, n = points.size(); i < n; ++i) {
		if (sm::dis_pos_to_pos(points[i], pos) < GetNodeRadius()) {
			return i;
		}
	}
	return -1;
}

bool PointsMesh::InsertInnerPos(const sm::vec2& pos)
{
	if (!m_mesh) {
		return false;
	}

	assert(m_mesh->Type() == pm::MESH_POINTS);
	pm::PointsMesh* points_mesh = static_cast<pm::PointsMesh*>(m_mesh);	

	const std::vector<sm::vec2>& outline = points_mesh->GetOutline();
	for (int i = 0, n = outline.size(); i < n; ++i) {
		if (sm::dis_pos_to_pos(outline[i], pos) < GetNodeRadius()) {
			return false;
		}
	}
	if (sm::is_point_intersect_polyline(pos, outline) || !sm::is_point_in_area(pos, outline)) {
		return false;
	}

	std::vector<sm::vec2> points = points_mesh->GetPoints();
	points.push_back(pos);
	points_mesh->Build(outline, points);

	return true;
}

bool PointsMesh::RemoveInnerPos(const sm::vec2& pos)
{
	if (!m_mesh) {
		return false;
	}

	assert(m_mesh->Type() == pm::MESH_POINTS);
	pm::PointsMesh* points_mesh = static_cast<pm::PointsMesh*>(m_mesh);	

	std::vector<sm::vec2> points = points_mesh->GetPoints();
	for (int i = 0, n = points.size(); i < n; ++i) 
	{
		const sm::vec2& p = points[i];
		if (sm::dis_pos_to_pos(p, pos) < GetNodeRadius()) 
		{
			points.erase(points.begin() + i);
			const std::vector<sm::vec2>& outline = points_mesh->GetOutline();
			points_mesh->Build(outline, points);
			return true;
		}
	}

	return false;
}

bool PointsMesh::MoveInnerPos(int idx, const sm::vec2& pos)
{
	if (!m_mesh) {
		return false;
	}

	assert(m_mesh->Type() == pm::MESH_POINTS);
	pm::PointsMesh* points_mesh = static_cast<pm::PointsMesh*>(m_mesh);	
	if (idx < 0 || idx >= points_mesh->GetPoints().size()) {
		return false;
	}

	std::vector<sm::vec2> points = points_mesh->GetPoints();
	points[idx] = pos;
	points_mesh->Build(points_mesh->GetOutline(), points);

	return true;
}

void PointsMesh::GetInnerPoints(std::vector<sm::vec2>& points) const
{
	if (!m_mesh) {
		return;
	}

	assert(m_mesh->Type() == pm::MESH_POINTS);
	pm::PointsMesh* points_mesh = static_cast<pm::PointsMesh*>(m_mesh);	
	points = points_mesh->GetPoints();
}

void PointsMesh::UpdateMeshFromShape()
{
	if (!m_mesh) {
		return;
	}

	assert(m_mesh->Type() == pm::MESH_POINTS);
	pm::PointsMesh* points_mesh = static_cast<pm::PointsMesh*>(m_mesh);
	points_mesh->Build(m_vertices, points_mesh->GetPoints());
}

}