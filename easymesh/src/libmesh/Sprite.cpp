#include "Sprite.h"
#include "Symbol.h"
#include "FileIO.h"
#include "Mesh.h"
#include "PropertySetting.h"

#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>

#include <polymesh/Mesh.h>
#include <s2loader/MeshIO.h>

namespace emesh
{

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::MeshSprite(spr)
	, ee::Sprite(spr)
{
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	s2::MeshSprite::operator = (spr);
	ee::Sprite::operator = (spr);
	return *this;
}

Sprite::Sprite(const s2::SymPtr& sym, uint32_t id)
	: s2::Sprite(sym)
	, s2::MeshSprite(sym)
	, ee::Sprite(sym)
{
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);
	
	const Json::Value& mesh_val = val["mesh"];
	auto& mesh = std::dynamic_pointer_cast<Symbol>(m_sym)->GetMesh();
	// reset
	pm::MeshTransform trans;
	mesh->GetMesh()->LoadFromTransform(trans);
	s2loader::MeshIO::Load(mesh_val, m_trans, *mesh);
	mesh->GetMesh()->LoadFromTransform(m_trans);

	if (!mesh_val["base_symbol"].isNull()) {
		std::string path = ee::FileHelper::GetAbsolutePath(dir, mesh_val["base_symbol"].asString());
		m_base = ee::SymbolMgr::Instance()->FetchSymbol(path);
	}
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::Sprite::Store(val);

	Json::Value mesh_val;

	auto& mesh = std::dynamic_pointer_cast<Symbol>(m_sym)->GetMesh();
	s2loader::MeshIO::Store(mesh_val, m_trans, *mesh);

	mesh_val["base_symbol"] = ee::FileHelper::GetRelativePath(dir, 
		std::dynamic_pointer_cast<const ee::Symbol>(m_base)->GetFilepath());

	val["mesh"] = mesh_val;
}

ee::PropertySetting* Sprite::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new PropertySetting(stage, std::dynamic_pointer_cast<Sprite>(shared_from_this()));
}

ee::SprPtr Sprite::Create(const std::shared_ptr<ee::Symbol>& sym) 
{
	return std::make_shared<Sprite>(sym);
}

}