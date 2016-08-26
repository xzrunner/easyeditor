#include "MeshSymbol.h"
#include "MeshSprite.h"
#include "Mesh.h"
#include "S2_Sprite.h"
#include "RenderParams.h"
#include "MeshTransform.h"
#include "DrawMesh.h"

#include <shaderlab.h>

namespace s2
{

MeshSymbol::MeshSymbol()
	: m_mesh(NULL)
	, m_pause(false)
{
}

MeshSymbol::~MeshSymbol()
{
	if (m_mesh) {
		m_mesh->RemoveReference();
	}
}

void MeshSymbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	if (!m_mesh) {
		return;
	}

	RenderParams p = params;
	if (spr) {
		p.mt = spr->GetTransMatrix() * params.mt;
		p.color = spr->Color() * params.color;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
	shader->SetColor(p.color.mul.ToABGR(), p.color.add.ToABGR());
	shader->SetColorMap(p.color.rmap.ToABGR(), p.color.gmap.ToABGR(), p.color.bmap.ToABGR());

	const MeshSprite* mesh_spr = VI_DOWNCASTING<const MeshSprite*>(spr);
	if (mesh_spr) {
		const MeshTransform& mtrans = mesh_spr->GetMeshTrans();
		mtrans.StoreToMesh(m_mesh);
	}

	if (mesh_spr && mesh_spr->OnlyDrawBound()) {
		DrawMesh::DrawInfoXY(m_mesh, &p.mt);
	} else {
		DrawMesh::DrawTexture(m_mesh, p, mesh_spr ? mesh_spr->GetBaseSym() : NULL);
	}

	if (!m_pause && mesh_spr)
	{
		sm::vec2 spd = mesh_spr->GetSpeed();
		if (spd.x != 0 || spd.y != 0) {
			m_mesh->OffsetUV(spd.x, spd.y);
		}
	}
}

sm::rect MeshSymbol::GetBounding(const Sprite* spr) const
{
	if (m_mesh) {
		return m_mesh->GetRegion();
	} else {
		return sm::rect();
	}
}

void MeshSymbol::SetMesh(Mesh* mesh)
{
	cu::RefCountObjAssign(m_mesh, mesh);
}

}