#include "Sprite.h"
#include "Symbol.h"
#include "FileIO.h"
#include "Mesh.h"
#include "PropertySetting.h"

#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>

#include <gum/MeshIO.h>

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

Sprite::Sprite(Symbol* sym)
	: s2::Sprite(sym)
	, s2::MeshSprite(sym)
	, ee::Sprite(sym)
{
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);
	
	const Json::Value& mesh_val = val["mesh"];
	const s2::Mesh* mesh = dynamic_cast<Symbol*>(m_sym)->GetMesh();
	gum::MeshIO::Load(mesh_val, m_trans, *mesh);
	m_trans.StoreToMesh(mesh);

	if (!mesh_val["base_symbol"].isNull()) {
		m_base->RemoveReference();
		std::string path = ee::FileHelper::GetAbsolutePath(dir, mesh_val["base_symbol"].asString());
		m_base = ee::SymbolMgr::Instance()->FetchSymbol(path);
	}
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::Sprite::Store(val);

	Json::Value mesh_val;

//	m_trans.LoadFromMesh(m_sym->GetMesh());
	const s2::Mesh* mesh = dynamic_cast<Symbol*>(m_sym)->GetMesh();
	gum::MeshIO::Store(mesh_val, m_trans, *mesh);

	const ee::Symbol* sym = dynamic_cast<const ee::Symbol*>(m_base);
	mesh_val["base_symbol"] = ee::FileHelper::GetRelativePath(dir, sym->GetFilepath());

	val["mesh"] = mesh_val;
}

ee::PropertySetting* Sprite::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new PropertySetting(stage, this);
}

ee::Sprite* Sprite::Create(ee::Symbol* sym) 
{
	return new Sprite(static_cast<Symbol*>(sym));
}

}