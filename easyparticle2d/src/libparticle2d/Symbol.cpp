#include "Symbol.h"
#include "Sprite.h"
#include "ParticleSystem.h"
#include "FileIO.h"
#include "PSConfigMgr.h"

#include <ps_2d.h>
#include <shaderlab.h>
#include <sprite2/RenderParams.h>

namespace eparticle2d
{

Symbol::Symbol()
	: m_et_cfg(NULL)
{
}

void Symbol::Draw(const s2::RenderParams& params, const s2::Sprite* spr) const
{
	if (!spr) {
		return;
	}

	Sprite* p2d_spr = const_cast<Sprite*>(dynamic_cast<const Sprite*>(spr));

	s2::RenderParams p = params;
	p.mt = p2d_spr->GetTransMatrix() * params.mt;
	p.color = spr->Color() * params.color;

	p2d_spr->SetMatrix(p.mt);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(p.color.mul.ToABGR(), p.color.add.ToABGR());
	shader->SetColorMap(p.color.rmap.ToABGR(), p.color.gmap.ToABGR(), p.color.bmap.ToABGR());

	p2d_spr->Draw(p.mt);		
}

sm::rect Symbol::GetBounding(const s2::Sprite* spr) const
{
	return sm::rect(sm::vec2(0, 0), 200, 200);
}

void Symbol::ReloadTexture() const
{
	for (int i = 0; i < m_et_cfg->sym_count; ++i) {
		ee::Symbol* sym = static_cast<ee::Symbol*>(m_et_cfg->syms[i].ud);
		sym->ReloadTexture();
	}
}

void Symbol::LoadResources()
{
	m_et_cfg = PSConfigMgr::Instance()->GetConfig(m_filepath);
}

}