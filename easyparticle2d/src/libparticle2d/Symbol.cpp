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

Symbol::Symbol(const Symbol& s)
	: ee::Symbol(s)
	, m_et_cfg(s.m_et_cfg)
{
}

Symbol* Symbol::Clone() const
{
	return new Symbol(*this);
}

void Symbol::Draw(const s2::RenderParams& params, const ee::Sprite* spr) const
{
	if (!spr) {
		return;
	}

	Sprite* p2d_spr = const_cast<Sprite*>(static_cast<const Sprite*>(spr));
	p2d_spr->SetMatrix(params.mt);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(params.color.mul.ToABGR(), params.color.add.ToABGR());
	shader->SetColorMap(params.color.rmap.ToABGR(), params.color.gmap.ToABGR(), params.color.bmap.ToABGR());

	p2d_spr->Draw(params.mt);		
}

void Symbol::ReloadTexture() const
{
	for (int i = 0; i < m_et_cfg->symbol_count; ++i) {
		ee::Symbol* symbol = static_cast<ee::Symbol*>(m_et_cfg->symbols[i].ud);
		symbol->ReloadTexture();
	}
}

sm::rect Symbol::GetSize(const ee::Sprite* sprite) const
{
	return sm::rect(sm::vec2(0, 0), 200, 200);
}

void Symbol::LoadResources()
{
	m_et_cfg = PSConfigMgr::Instance()->GetConfig(m_filepath);
}

}