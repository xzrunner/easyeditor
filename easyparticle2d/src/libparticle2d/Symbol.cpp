#include "Symbol.h"
#include "Sprite.h"
#include "ParticleSystem.h"
#include "FileIO.h"
#include "PSConfigMgr.h"

#include <ps_2d.h>

#include <ee/ShaderMgr.h>
#include <ee/SpriteShader.h>

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

void Symbol::Draw(const ee::Matrix& mt, const ee::ColorTrans& color, 
				  const ee::Sprite* spr, const ee::Sprite* root) const
{
	if (!spr) {
		return;
	}

	Sprite* p2d_spr = const_cast<Sprite*>(static_cast<const Sprite*>(spr));
	p2d_spr->SetMatrix(mt);

	ee::ShaderMgr* mgr = ee::ShaderMgr::Instance();
	ee::SpriteShader* shader = static_cast<ee::SpriteShader*>(mgr->GetShader(ee::ShaderMgr::SPRITE));
	shader->SetColor(color);

	p2d_spr->Draw(mt);		
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