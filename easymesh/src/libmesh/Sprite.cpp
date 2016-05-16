#include "Sprite.h"
#include "FileIO.h"
#include "Mesh.h"

#include <ee/BBFactory.h>
#include <ee/BoundingBox.h>

#include <sprite2/DummySprite.h>

namespace emesh
{

Sprite::Sprite()
	: m_symbol(NULL)
{
	m_core = new s2::DummySprite();

//	m_speed.set(0, -0.01f);
}

Sprite::Sprite(const Sprite& s)
	: ee::Sprite(s)
	, m_speed(s.m_speed)
	, m_trans(s.m_trans)
{
	m_core = new s2::DummySprite();

	m_symbol = s.m_symbol;
	m_symbol->Retain();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
{
	m_core = new s2::DummySprite();

//	m_speed.set(0, -0.01f);

	m_trans.LoadFromMesh(symbol->GetMesh());

	m_symbol->Retain();
	BuildBounding();
}

Sprite::~Sprite()
{
	delete m_core; m_core = NULL;

	if (m_symbol) {
		m_symbol->Release();
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

void Sprite::Load(const Json::Value& val)
{
	ee::Sprite::Load(val);
	
	const Json::Value& mesh_val = val["mesh"];
	m_trans.Load(mesh_val);
	m_trans.StoreToMesh(m_symbol->GetMesh());
}

void Sprite::Store(Json::Value& val) const
{
	ee::Sprite::Store(val);

	Json::Value mesh_val;
//	m_trans.LoadFromMesh(m_symbol->GetMesh());
	m_trans.Store(mesh_val);
	val["mesh"] = mesh_val;
}

void Sprite::SetTween(Sprite* begin, Sprite* end, float process)
{
	GetMeshTrans().SetTween(begin->GetMeshTrans(), end->GetMeshTrans(), process);
}

}