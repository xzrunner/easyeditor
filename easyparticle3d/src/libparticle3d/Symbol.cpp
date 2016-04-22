#include "Symbol.h"
#include "Sprite.h"
#include "ParticleSystem.h"
#include "FileIO.h"
#include "PSConfigMgr.h"
#include "PS.h"

#include <ee/RenderParams.h>
#include <ee/trans_color.h>

#include <ps_3d.h>
#include <ps_3d_sprite.h>
#include <shaderlab.h>

#include <fstream>

namespace eparticle3d
{

Symbol::Symbol()
	: m_et_cfg(NULL)
	, m_loop(true)
	, m_local(true)
{
	PS::Instance();
}

Symbol::Symbol(const Symbol& s)
	: ee::Symbol(s)
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

	Sprite* p3d_spr = const_cast<Sprite*>(static_cast<const Sprite*>(spr));
	p3d_spr->SetMatrix(trans.mt);
	if (p3d_spr->IsAlone()) {
		p3d_sprite* p3d = p3d_spr->GetP3D();
		if (!p3d) {
			return;
		}
		RenderParams* rp = static_cast<RenderParams*>(p3d->draw_params);
		rp->mat = trans.mt;
		rp->ct = trans.color;
		const float* src = trans.mt.GetElements();
		p3d->mat[0] = src[0];
		p3d->mat[1] = src[1];
		p3d->mat[2] = src[4];
		p3d->mat[3] = src[5];
		p3d->mat[4] = src[12];
		p3d->mat[5] = src[13];	
		return;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(ee::color2int(trans.color.multi, ee::PT_ABGR),
		ee::color2int(trans.color.add, ee::PT_ABGR));
	shader->SetColorMap(ee::color2int(trans.color.r, ee::PT_ABGR),
		ee::color2int(trans.color.g, ee::PT_ABGR),
		ee::color2int(trans.color.b, ee::PT_ABGR));

	p3d_spr->Draw(trans.mt);
}

ee::Rect Symbol::GetSize(const ee::Sprite* sprite) const
{
	return ee::Rect(200, 200);
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

	m_loop = value["loop"].asBool();
	m_local = value["local"].asBool();
}

}