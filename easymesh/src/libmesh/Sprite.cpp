#include "Sprite.h"
#include "Symbol.h"
#include "FileIO.h"
#include "Mesh.h"
#include "PropertySetting.h"

#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>

namespace emesh
{

Sprite::Sprite()
	: m_base(NULL)
	, m_only_draw_bound(false)
{
//	m_speed.set(0, -0.01f);
}

Sprite::Sprite(const Sprite& s)
	: ee::Sprite(s)
	, m_speed(s.m_speed)
	, m_trans(s.m_trans)
	, m_only_draw_bound(s.m_only_draw_bound)
{
	if (m_base = s.m_base) {
		m_base->AddReference();
	}
}

Sprite::Sprite(Symbol* sym)
	: ee::Sprite(sym)
	, m_only_draw_bound(false)
{
	if (m_base = sym->GetMesh()->GetBaseSymbol()) {
		m_base->AddReference();
	}

//	m_speed.set(0, -0.01f);

	m_trans.LoadFromMesh(sym->GetMesh());
}

Sprite::~Sprite()
{
	if (m_base) {
		m_base->RemoveReference();
	}
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);
	
	const Json::Value& mesh_val = val["mesh"];
	m_trans.Load(mesh_val);
	m_trans.StoreToMesh(dynamic_cast<Symbol*>(m_sym)->GetMesh());

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

//	m_trans.LoadFromMesh(m_symbol->GetMesh());
	m_trans.Store(mesh_val);
	
	mesh_val["base_symbol"] = ee::FileHelper::GetRelativePath(dir, m_base->GetFilepath());

	val["mesh"] = mesh_val;
}

ee::PropertySetting* Sprite::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new PropertySetting(stage, this);
}

void Sprite::SetTween(Sprite* begin, Sprite* end, float process)
{
	GetMeshTrans().SetTween(begin->GetMeshTrans(), end->GetMeshTrans(), process);
}

void Sprite::SetBaseSym(const ee::Symbol* sym) 
{ 
	cu::RefCountObjAssign(m_base, sym); 
}

ee::Sprite* Sprite::Create(ee::Symbol* sym) 
{
	return new Sprite(static_cast<Symbol*>(sym));
}

}