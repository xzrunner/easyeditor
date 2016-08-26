#include "Symbol.h"
#include "Sprite.h"
#include "ParticleSystem.h"
#include "FileIO.h"
#include "PSConfigMgr.h"
#include "PSNode.h"

#include <ps_3d.h>
#include <ps_3d_sprite.h>
#include <shaderlab.h>
#include <sprite2/RenderParams.h>

#include <assert.h>

#include <fstream>

namespace eparticle3d
{

Symbol::Symbol()
	: m_et_cfg(NULL)
	, m_et(NULL)
	, m_loop(true)
	, m_local(true)
{
	PSNode::Instance();
}

bool Symbol::Update(const s2::RenderParams& params, float _dt)
{
	PSNode::Instance()->UpdateTime();

	float time = PSNode::Instance()->GetTime();
	assert(m_et->time <= time);
	if (m_et->time == time) {
		return false;
	}

	float mt[6];
	sm::mat4 inner_mat;
	mt[0] = inner_mat.x[0];
	mt[1] = inner_mat.x[1];
	mt[2] = inner_mat.x[4];
	mt[3] = inner_mat.x[5];
	mt[4] = inner_mat.x[12];
	mt[5] = inner_mat.x[13];	

	float dt = time - m_et->time;
	p3d_emitter_update(m_et, dt, mt);
	m_et->time = time;

	return true;
}

void Symbol::Draw(const s2::RenderParams& params, const s2::Sprite* spr) const
{
	if (!spr) {
		sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
		sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
		shader->SetColor(params.color.mul.ToABGR(), params.color.add.ToABGR());
		shader->SetColorMap(params.color.rmap.ToABGR(), params.color.gmap.ToABGR(), params.color.bmap.ToABGR());
		RenderParams rp;
		rp.mat = params.mt;
		rp.ct = params.color;
		rp.p3d = NULL;
		p3d_emitter_draw(m_et, &rp);
		return;
	}

	s2::RenderParams p = params;
	p.color = spr->Color() * params.color;

	Sprite* p3d_spr = const_cast<Sprite*>(dynamic_cast<const Sprite*>(spr));
	p3d_spr->SetOuterMatrix(p.mt);
	if (p3d_spr->IsAlone()) {
		p3d_sprite* p3d = p3d_spr->GetP3D();
		if (!p3d) {
			return;
		}
		RenderParams* rp = static_cast<RenderParams*>(p3d->draw_params);
		rp->mat = p.mt;
		rp->ct = p.color;
		sm::mat4 mt = p3d_spr->GetTransMatrix() * p.mt;
		p3d->mat[0] = mt.x[0];
		p3d->mat[1] = mt.x[1];
		p3d->mat[2] = mt.x[4];
		p3d->mat[3] = mt.x[5];
		p3d->mat[4] = mt.x[12];
		p3d->mat[5] = mt.x[13];	
		return;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(p.color.mul.ToABGR(), p.color.add.ToABGR());
	shader->SetColorMap(p.color.rmap.ToABGR(), p.color.gmap.ToABGR(), p.color.bmap.ToABGR());

	if (p3d_spr->IsLocalModeDraw()) {
		p.mt = p3d_spr->GetTransMatrix() * p.mt;
	}
	p3d_spr->Draw(p);
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

void Symbol::Traverse(ee::Visitor<ee::Sprite>& visitor)
{
	if (!m_et_cfg) {
		return;
	}
	for (int i = 0; i < m_et_cfg->sym_count; ++i)
	{
		const p3d_symbol& p_symbol = m_et_cfg->syms[i];
		ee::Symbol* sym = static_cast<ee::Symbol*>(p_symbol.ud);
		sym->Traverse(visitor);
	}
}

void Symbol::LoadResources()
{
	m_et_cfg = PSConfigMgr::Instance()->GetConfig(m_filepath);
	m_et = p3d_emitter_create(m_et_cfg);
	p3d_emitter_start(m_et);

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