#include "Symbol.h"
#include "Mesh.h"
#include "FileIO.h"
#include "Sprite.h"
#include "MeshFactory.h"
#include "MeshRenderer.h"

#include <ee/Image.h>

#include <sprite2/RenderParams.h>

#include <shaderlab.h>

#include <assert.h>

namespace emesh
{

Symbol::Symbol()
	: m_mesh(NULL)
	, m_pause(false)
{
}

Symbol::Symbol(const Symbol& s)
	: ee::Symbol(s)
{
	m_mesh = s.m_mesh->Clone();
}

Symbol::Symbol(ee::Symbol* base)
{
	m_mesh = MeshFactory::Instance()->CreateMesh(base);
}

Symbol::~Symbol()
{
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

	s2::RenderParams p = params;
	if (spr) {
		p.mt = spr->GetTransMatrix() * params.mt;
		p.color = spr->GetColor() * params.color;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
	shader->SetColor(p.color.mul.ToABGR(), p.color.add.ToABGR());
	shader->SetColorMap(p.color.rmap.ToABGR(), p.color.gmap.ToABGR(), p.color.bmap.ToABGR());

	const Sprite* mesh_spr = static_cast<const Sprite*>(spr);
	if (mesh_spr) {
		const MeshTrans& mtrans = mesh_spr->GetMeshTrans();
		mtrans.StoreToMesh(m_mesh);
	}

	if (mesh_spr && mesh_spr->OnlyDrawBound()) {
		MeshRenderer::DrawInfoXY(m_mesh, &p.mt);
 	} else {
 		MeshRenderer::DrawTexture(m_mesh, p, mesh_spr ? mesh_spr->GetBaseSym() : NULL);
 	}

	if (!m_pause && mesh_spr)
	{
		sm::vec2 spd = mesh_spr->GetSpeed();
		if (spd.x != 0 || spd.y != 0) {
			m_mesh->OffsetUV(spd.x, spd.y);
		}
	}
}

void Symbol::ReloadTexture() const
{
	if (const ee::Symbol* sym = m_mesh->GetBaseSymbol()) {
		sym->ReloadTexture();
	}
}

// sm::rect Symbol::getSize(const ee::Sprite* sprite) const
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