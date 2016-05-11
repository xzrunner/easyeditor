#include "Symbol.h"
#include "Mesh.h"
#include "FileIO.h"
#include "Sprite.h"
#include "MeshFactory.h"
#include "MeshRenderer.h"

#include <ee/Image.h>

#include <sprite2/RenderParams.h>

#include <shaderlab.h>

namespace emesh
{

Symbol::Symbol()
	: m_base(NULL)
	, m_mesh(NULL)
	, m_pause(false)
{
}

Symbol::Symbol(const Symbol& s)
	: ee::Symbol(s)
{
	m_base = s.m_base;
	m_base->Retain();

	m_mesh = s.m_mesh->Clone();
}

Symbol::Symbol(ee::Symbol* base)
{
	m_base = base;
	m_base->Retain();

	m_mesh = MeshFactory::Instance()->CreateMesh(m_base);
}

Symbol::~Symbol()
{
	if (m_base)
	{
		m_base->Release();
		m_base = NULL;
	}
	if (m_mesh)
	{
		m_mesh->Release();
		m_mesh = NULL;
	}
}

Symbol* Symbol::Clone() const 
{ 
	return new Symbol(*this);
}

void Symbol::Draw(const s2::RenderParams& params, const ee::Sprite* spr) const
{
	if (!m_mesh) {
		return;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
	shader->SetColor(params.color.mul.ToABGR(), params.color.add.ToABGR());
	shader->SetColorMap(params.color.rmap.ToABGR(), params.color.gmap.ToABGR(), params.color.bmap.ToABGR());

	if (spr) {
		const MeshTrans& mtrans = static_cast<const Sprite*>(spr)->GetMeshTrans();
		mtrans.StoreToMesh(m_mesh);
	}
	MeshRenderer::DrawTexture(m_mesh, params);
	if (!m_pause && spr)
	{
		const Sprite* s = static_cast<const Sprite*>(spr);
		sm::vec2 spd = s->GetSpeed();
		if (spd.x != 0 || spd.y != 0) {
			m_mesh->OffsetUV(spd.x, spd.y);
		}
	}
}

void Symbol::ReloadTexture() const
{
	if (m_base) {
		m_base->ReloadTexture();
	}
}

// ee::Rect Symbol::getSize(const ee::Sprite* sprite) const
// {
// //	return m_image->getRegion();
// }

void Symbol::SetMesh(Mesh* mesh)
{
	if (m_mesh) {
		m_mesh->Release();
	}
	mesh->Retain();
	m_mesh = mesh;
}

void Symbol::CreateMesh()
{
	if (m_mesh) {
		m_mesh->Release();
	}
	m_mesh = MeshFactory::Instance()->CreateMesh(m_base);
}

void Symbol::LoadResources()
{
	FileIO::Load(m_filepath.c_str(), this);
	InitBounding();
}

void Symbol::InitBounding()
{
	m_region = m_mesh->GetRegion();
}

}