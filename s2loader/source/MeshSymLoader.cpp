#include "s2loader/MeshSymLoader.h"
#include "s2loader/MeshIO.h"
#include "s2loader/ArrayLoader.h"

#include <simp/NodeMesh.h>
#include <simp/PointsMesh.h>
#include <simp/TrianglesMesh.h>
#include <simp/Skin2Mesh.h>
#include <polymesh/PointsMesh.h>
#include <polymesh/TrianglesMesh.h>
#include <polymesh/Skin2Mesh.h>
#include <polymesh/MeshTransform.h>
#include <sprite2/MeshSymbol.h>
#include <sprite2/Mesh.h>
#include <bs/FixedPointNum.h>
#include <gum/FilepathHelper.h>
#include <gum/JsonSerializer.h>
#include <gum/SymbolPool.h>

#include <json/json.h>

#include <fstream>

namespace s2loader
{

MeshSymLoader::MeshSymLoader(s2::MeshSymbol& sym)
	: m_sym(sym)
{
}

void MeshSymLoader::LoadJson(const CU_STR& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	if (!value.isMember("base_symbol")) {
		return;
	}

	CU_STR dir = gum::FilepathHelper::Dir(filepath);
	CU_STR base_path = gum::FilepathHelper::Absolute(dir, value["base_symbol"].asString().c_str());
	auto base_sym = gum::SymbolPool::Instance()->Fetch(base_path);
	if (!base_sym) {
		return;
	}

	CU_STR type = value["type"].asString().c_str();
	std::unique_ptr<s2::Mesh> mesh;
	if (type == "strip") {

	} else if (type == "network" || type == "points") {
		mesh = CreatePointsMesh(value, base_sym);
	}

	if (mesh) {
		m_sym.SetMesh(mesh);
	}
}

void MeshSymLoader::LoadBin(const simp::NodeMesh* node)
{
	auto base_sym = gum::SymbolPool::Instance()->Fetch(node->base_id);
	if (!base_sym) {
		return;
	}

	std::unique_ptr<s2::Mesh> mesh = nullptr;
	switch (node->shape->type)
	{
	case simp::MESH_POINTS:
		mesh = LoadPointsMesh(base_sym, static_cast<simp::PointsMesh*>(node->shape));
		break;
	case simp::MESH_TRIANGLES:
		mesh = LoadTrianglesMesh(base_sym, static_cast<simp::TrianglesMesh*>(node->shape));
		break;
	case simp::MESH_SKIN2:
		mesh = LoadSkin2Mesh(base_sym, static_cast<simp::Skin2Mesh*>(node->shape));
		break;
	default:
		break;
	}
	m_sym.SetMesh(mesh);
}

std::unique_ptr<s2::Mesh> MeshSymLoader::LoadPointsMesh(const s2::SymConstPtr& base_sym, simp::PointsMesh* node)
{
	auto s2_mesh = std::make_unique<s2::Mesh>(base_sym);
	
	CU_VEC<sm::vec2> outline;
	ArrayLoader::Load(outline, node->outline, node->outline_n, 16);

	CU_VEC<sm::vec2> points;
	ArrayLoader::Load(points, node->points, node->points_n, 16);

	sm::rect r = base_sym->GetBounding();
	auto pm_mesh = std::make_unique<pm::PointsMesh>(outline, points, r.Width(), r.Height());
	s2_mesh->SetMesh(std::move(pm_mesh));
	
	return s2_mesh;
}

std::unique_ptr<s2::Mesh> MeshSymLoader::LoadTrianglesMesh(const s2::SymConstPtr& base_sym, simp::TrianglesMesh* node)
{
	auto s2_mesh = std::make_unique<s2::Mesh>(base_sym);

	CU_VEC<sm::vec2> vertices;
	ArrayLoader::Load(vertices, node->vertices, node->vertices_n, 16);

	CU_VEC<sm::vec2> texcoords;
	ArrayLoader::Load(texcoords, node->texcoords, node->texcoords_n, 8192);

	CU_VEC<int> triangles;
	ArrayLoader::Load(triangles, node->triangle, node->triangle_n);

	auto pm_mesh = std::make_unique<pm::TrianglesMesh>(vertices, texcoords, triangles);
	s2_mesh->SetMesh(std::move(pm_mesh));

	return s2_mesh;
}

std::unique_ptr<s2::Mesh> MeshSymLoader::LoadSkin2Mesh(const s2::SymConstPtr& base_sym, simp::Skin2Mesh* node)
{
	auto s2_mesh = std::make_unique<s2::Mesh>(base_sym);

	CU_VEC<pm::Skin2Joint> joints;
	int joints_n = 0;
	for (uint32_t i = 0; i < node->vertices_n; ++i) {
		joints_n += node->joints_n[i];
	}
	joints.reserve(joints_n);
	for (int i = 0; i < joints_n; ++i) 
	{
		const simp::Skin2Mesh::Joint& src = node->joints[i];
		pm::Skin2Joint dst;
		dst.joint = src.joint;
		dst.vertex.x = bs::int2float(src.vx, 16);
		dst.vertex.y = bs::int2float(src.vy, 16);
		dst.offset.Set(0, 0);
		dst.weight = bs::int2float(src.weight, 2048) + 0.5f;
		joints.push_back(dst);
	}

	CU_VEC<int> vertices;
	vertices.reserve(node->vertices_n);
	for (uint32_t i = 0; i < node->vertices_n; ++i) {
		vertices.push_back(node->joints_n[i]);
	}

	CU_VEC<sm::vec2> texcoords;
	ArrayLoader::Load(texcoords, node->texcoords, node->texcoords_n, 8192);

	CU_VEC<int> triangles;
	ArrayLoader::Load(triangles, node->triangle, node->triangle_n);

	auto pm_mesh = std::make_unique<pm::Skin2Mesh>(joints, vertices, texcoords, triangles);
	s2_mesh->SetMesh(std::move(pm_mesh));

	return s2_mesh;
}

std::unique_ptr<s2::Mesh> MeshSymLoader::CreatePointsMesh(const Json::Value& val, const s2::SymConstPtr& base_sym)
{
	auto s2_mesh = std::make_unique<s2::Mesh>(base_sym);

	CU_VEC<sm::vec2> outline;
	 gum::JsonSerializer::Load(val["shape"]["outline"], outline);

	CU_VEC<sm::vec2> points;
	 gum::JsonSerializer::Load(val["shape"]["inner"], points);

	sm::rect r = base_sym->GetBounding();
	auto pm_mesh = std::make_unique<pm::PointsMesh>(outline, points, r.Width(), r.Height());

	pm::MeshTransform trans;
	MeshIO::Load(val, trans, *s2_mesh);
	pm_mesh->LoadFromTransform(trans);

	s2_mesh->SetMesh(std::move(pm_mesh));

	return s2_mesh;
}

}