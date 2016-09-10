#include "MeshSymLoader.h"
#include "FilepathHelper.h"
#include "SymbolFactory.h"
#include "JsonSerializer.h"

#include <sprite2/MeshSymbol.h>
#include <sprite2/Mesh.h>
#include <sprite2/MeshTransform.h>
#include <sprite2/NetworkMesh.h>
#include <sprite2/NetworkShape.h>

#include <json/json.h>

#include <fstream>

namespace glue
{

MeshSymLoader::MeshSymLoader(s2::MeshSymbol* sym)
	: m_sym(sym)
{
	if (m_sym) {
		m_sym->AddReference();
	}
}

MeshSymLoader::~MeshSymLoader()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}
}

void MeshSymLoader::LoadJson(const std::string& filepath)
{
	if (!m_sym) {
		return;
	}

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

	std::string dir = FilepathHelper::Dir(filepath);
	std::string base_path = FilepathHelper::Absolute(dir, value["base_symbol"].asString());
	s2::Symbol* base_sym = SymbolFactory::Instance()->Create(base_path);

	std::string type = value["type"].asString();
	s2::Mesh* mesh = NULL;
	if (type == "strip") {

	} else if (type == "network") {
		mesh = CreateNetworkMesh(value, base_sym);
	}

	m_sym->SetMesh(mesh);
	mesh->RemoveReference();
}

void MeshSymLoader::LoadMeshTransform(s2::MeshTransform& trans, const Json::Value& value)
{
	std::map<sm::vec2, sm::vec2, sm::Vector2Cmp>& map = trans.GetMap();
	map.clear();
	std::vector<sm::vec2> from, to;
	JsonSerializer::Load(value["trans"]["from"], from);
	JsonSerializer::Load(value["trans"]["to"], to);
	for (int i = 0, n = from.size(); i < n; ++i) {
		map.insert(std::make_pair(from[i], to[i]));
	}
}

s2::Mesh* MeshSymLoader::CreateNetworkMesh(const Json::Value& val, 
										   const s2::Symbol* sym)
{
	s2::NetworkMesh* mesh = new s2::NetworkMesh(sym);

	std::vector<sm::vec2> outline;
	JsonSerializer::Load(val["shape"]["outline"], outline);
	s2::NetworkShape* shape = new s2::NetworkShape(outline);
	
	std::vector<sm::vec2> inner;
	JsonSerializer::Load(val["shape"]["inner"], inner);
	shape->SetInnerVertices(inner);

	mesh->SetShape(shape);
	shape->RemoveReference();

	s2::MeshTransform trans;
	LoadMeshTransform(trans, val);
	trans.StoreToMesh(mesh);

	return mesh;
}

}