#include "Particle3dSymbol.h"
#include "Particle3dSprite.h"
#include "Particle3d.h"
#include "S2_Sprite.h"

#include <shaderlab.h>
#include <ps_3d_sprite.h>

#include <assert.h>

namespace s2
{

Particle3dSymbol::Particle3dSymbol()
	: m_et_cfg(NULL)
	, m_et(NULL)
	, m_loop(true)
	, m_local(true)
{
	Particle3d::Instance();
}

Particle3dSymbol::~Particle3dSymbol()
{
	if (m_et) {
		p3d_emitter_release(m_et);
	}
}

bool Particle3dSymbol::Update(const RenderParams& params)
{
	float time = Particle3d::Instance()->GetTime();
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

void Particle3dSymbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	if (!spr) {
		sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
		sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
		shader->SetColor(params.color.mul.ToABGR(), params.color.add.ToABGR());
		shader->SetColorMap(params.color.rmap.ToABGR(), params.color.gmap.ToABGR(), params.color.bmap.ToABGR());
		P3dRenderParams rp;
		rp.mat = params.mt;
		rp.ct = params.color;
		rp.p3d = NULL;
		p3d_emitter_draw(m_et, &rp);
		return;
	}

	RenderParams p = params;
	p.color = spr->Color() * params.color;

	const Particle3dSprite* p3d_spr = VI_DOWNCASTING<const Particle3dSprite*>(spr);
	p3d_spr->SetOuterMatrix(p.mt);
	if (p3d_spr->IsAlone()) {
		p3d_sprite* p3d = p3d_spr->GetP3dSpr();
		if (!p3d) {
			return;
		}
		P3dRenderParams* rp = static_cast<P3dRenderParams*>(p3d->draw_params);
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

sm::rect Particle3dSymbol::GetBounding(const Sprite* spr) const
{
	return sm::rect(sm::vec2(0, 0), 200, 200);
}

}