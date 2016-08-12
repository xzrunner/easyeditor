#include "Sprite.h"
#include "FileIO.h"
#include "Mesh.h"
#include "PropertySetting.h"

#include <ee/BBFactory.h>
#include <ee/BoundingBox.h>
#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>

#include <sprite2/DummySprite.h>

namespace emesh
{

Sprite::Sprite()
	: m_symbol(NULL)
	, m_base(NULL)
	, m_only_draw_bound(false)
{
	m_core = new s2::DummySprite(this);

//	m_speed.set(0, -0.01f);
}

Sprite::Sprite(const Sprite& s)
	: ee::Sprite(s)
	, m_speed(s.m_speed)
	, m_trans(s.m_trans)
	, m_only_draw_bound(s.m_only_draw_bound)
{
	m_core = new s2::DummySprite(this);
	if (m_base = s.m_base) {
		m_base->AddReference();
	}

	m_symbol = s.m_symbol;
	m_symbol->AddReference();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
	, m_only_draw_bound(false)
{
	m_core = new s2::DummySprite(this);
	if (m_base = symbol->GetMesh()->GetBaseSymbol()) {
		m_base->AddReference();
	}

//	m_speed.set(0, -0.01f);

	m_trans.LoadFromMesh(symbol->GetMesh());

	m_symbol->AddReference();
	BuildBounding();
}

Sprite::~Sprite()
{
	m_core->RemoveReference();
	if (m_base) {
		m_base->RemoveReference();
	}
	if (m_symbol) {
		m_symbol->RemoveReference();
	}
}

Sprite* Sprite::Clone() const
{
	return new Sprite(*this);
}

const Symbol& Sprite::GetSymbol() const
{
	return *m_symbol;
}

void Sprite::SetSymbol(ee::Symbol* symbol)
{
	ee::Sprite::SetSymbol(&m_symbol, symbol);
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);
	
	const Json::Value& mesh_val = val["mesh"];
	m_trans.Load(mesh_val);
	m_trans.StoreToMesh(m_symbol->GetMesh());

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

}