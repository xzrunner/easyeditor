#include "MeshSprLoader.h"
#include "MeshSymLoader.h"
#include "FilepathHelper.h"
#include "SymbolFactory.h"

#include <sprite2/MeshSprite.h>
#include <sprite2/MeshSymbol.h>

namespace gum
{

MeshSprLoader::MeshSprLoader(s2::MeshSprite* spr)
	: m_spr(spr)
{
	if (m_spr) {
		m_spr->AddReference();
	}
}

MeshSprLoader::~MeshSprLoader()
{
	if (m_spr) {
		m_spr->RemoveReference();
	}
}

void MeshSprLoader::LoadJson(const Json::Value& val, const std::string& dir)
{
	if (!m_spr || !val.isMember("mesh")) {
		return;
	}

	const Json::Value& mesh_val = val["mesh"];

	s2::MeshTransform& trans = m_spr->GetMeshTrans();
	MeshSymLoader::LoadMeshTransform(trans, mesh_val);
	trans.StoreToMesh(VI_DOWNCASTING<s2::MeshSymbol*>(m_spr->GetSymbol())->GetMesh());

	if (mesh_val.isMember("base_symbol")) {
		std::string base_path = FilepathHelper::Absolute(dir, mesh_val["base_symbol"].asString());
		s2::Symbol* base_sym = SymbolFactory::Instance()->Create(base_path);
		m_spr->SetBaseSym(base_sym);
		base_sym->RemoveReference();
	}
}

}