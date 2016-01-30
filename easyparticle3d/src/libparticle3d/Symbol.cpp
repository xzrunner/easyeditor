#include "Symbol.h"
#include "Sprite.h"
#include "ParticleSystem.h"
#include "FileIO.h"
#include "PSConfigMgr.h"
#include "PS.h"

#include <ps_3d.h>

namespace eparticle3d
{

Symbol::Symbol()
	: m_et_cfg(NULL)
{
	PS::Instance();
}

Symbol::Symbol(const Symbol& s)
	: d2d::Symbol(s)
	, m_et_cfg(s.m_et_cfg)
{
	PS::Instance();
}

Symbol* Symbol::Clone() const
{
	return new Symbol(*this);
}

void Symbol::ReloadTexture() const
{
	for (int i = 0; i < m_et_cfg->symbol_count; ++i) {
		d2d::Symbol* symbol = static_cast<d2d::Symbol*>(m_et_cfg->symbols[i].ud);
		symbol->ReloadTexture();
	}
}

void Symbol::Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color, 
				  const d2d::Sprite* spr, const d2d::Sprite* root) const
{
	if (!spr) {
		return;
	}

	Sprite* p3d_spr = const_cast<Sprite*>(static_cast<const Sprite*>(spr));
	p3d_spr->SetMatrix(mt);
	if (p3d_spr->IsAlone()) {
		p3d_sprite* p3d = p3d_spr->GetP3D();
		if (!p3d) {
			return;
		}
		RenderParams* rp = static_cast<RenderParams*>(p3d->draw_params);
		rp->mat = mt;
		rp->ct = color;
		const float* src = mt.GetElements();
		p3d->mat[0] = src[0];
		p3d->mat[1] = src[1];
		p3d->mat[2] = src[4];
		p3d->mat[3] = src[5];
		p3d->mat[4] = src[12];
		p3d->mat[5] = src[13];	
		return;
	}

	d2d::ShaderMgr* shader = d2d::ShaderMgr::Instance();
	shader->SetSpriteColor(color);

	p3d_spr->Draw(mt);
}

d2d::Rect Symbol::GetSize(const d2d::Sprite* sprite) const
{
	return d2d::Rect(200, 200);
}

void Symbol::LoadResources()
{
	m_et_cfg = PSConfigMgr::Instance()->GetConfig(m_filepath);
	m_et = p3d_emitter_create(m_et_cfg);

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	name = value["name"].asString();
}

}