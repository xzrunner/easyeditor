#include "MeshSprite.h"
#include "MeshTriangle.h"
#include "MeshSymbol.h"
#include "Mesh.h"

#include <stddef.h>

namespace s2
{

MeshSprite::MeshSprite() 
	: m_base(NULL)
	, m_only_draw_bound(false)
{
//	m_speed.set(0, -0.01f);
}

MeshSprite::MeshSprite(const MeshSprite& mesh)
	: Sprite(mesh)
	, m_speed(mesh.m_speed)
	, m_trans(mesh.m_trans)
	, m_only_draw_bound(mesh.m_only_draw_bound)
{
	if (m_base = mesh.m_base) {
		m_base->AddReference();
	}
}

MeshSprite& MeshSprite::operator = (const MeshSprite& mesh)
{
	Sprite::operator = (mesh);
	m_speed = mesh.m_speed;
	m_trans = mesh.m_trans;
	cu::RefCountObjAssign(m_base, const_cast<const Symbol*>(mesh.m_base));
	m_only_draw_bound = mesh.m_only_draw_bound;
	return *this;
}

MeshSprite::MeshSprite(Symbol* sym) 
	: Sprite(sym) 
	, m_only_draw_bound(false)
{
	Mesh* mesh = VI_DOWNCASTING<MeshSymbol*>(sym)->GetMesh();
	if (m_base = mesh->GetBaseSymbol()) {
		m_base->AddReference();
	}

//	m_speed.set(0, -0.01f);

	m_trans.LoadFromMesh(mesh);
}

MeshSprite::~MeshSprite()
{
	if (m_base) {
		m_base->RemoveReference();
	}
}

MeshSprite* MeshSprite::Clone() const
{
	return new MeshSprite(*this);
}

void MeshSprite::SetTween(MeshSprite* begin, MeshSprite* end, float process)
{
	m_trans.SetTween(begin->GetMeshTrans(), end->GetMeshTrans(), process);
}

void MeshSprite::SetBaseSym(const Symbol* sym) 
{ 
	cu::RefCountObjAssign(m_base, sym); 
}

}