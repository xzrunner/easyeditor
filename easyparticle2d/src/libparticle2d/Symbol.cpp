#include "Symbol.h"
#include "Sprite.h"
#include "ParticleSystem.h"
#include "FileIO.h"
#include "PSConfigMgr.h"

#include <ee/RenderParams.h>

#include <ps_2d.h>
#include <shaderlab.h>

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

void Symbol::ReloadTexture() const
{
	for (int i = 0; i < m_et_cfg->symbol_count; ++i) {
		ee::Symbol* symbol = static_cast<ee::Symbol*>(m_et_cfg->symbols[i].ud);
		symbol->ReloadTexture();
	}
}

void Symbol::Draw(const ee::RenderParams& trans, const ee::Sprite* spr, 
				  const ee::Sprite* root) const
{
	if (!spr) {
		return;
	}

	Sprite* p2d_spr = const_cast<Sprite*>(static_cast<const Sprite*>(spr));
	p2d_spr->SetMatrix(trans.mt);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(trans.color.mul.ToABGR(), trans.color.add.ToABGR());
	shader->SetColorMap(trans.color.rmap.ToABGR(), trans.color.gmap.ToABGR(), trans.color.bmap.ToABGR());

	p2d_spr->Draw(trans.mt);		
}

ee::Rect Symbol::GetSize(const ee::Sprite* sprite) const
{
	return ee::Rect(200, 200);
}

void Symbol::LoadResources()
{
	m_et_cfg = PSConfigMgr::Instance()->GetConfig(m_filepath);
}

}