#include "TrailSymbol.h"
#include "TrailSprite.h"
#include "RenderParams.h"
#include "S2_Sprite.h"

#include <mt_2d.h>
#include <shaderlab.h>

namespace s2
{

TrailSymbol::TrailSymbol()
	: m_et_cfg(NULL)
	, m_et(NULL)
{
}

TrailSymbol::~TrailSymbol()
{
	if (m_et) {
		t2d_emitter_release(m_et);
	}
}

void TrailSymbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	if (!spr) {
		return;
	}

	RenderParams p = params;
	p.color = spr->Color() * params.color;

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(p.color.mul.ToABGR(), p.color.add.ToABGR());
	shader->SetColorMap(p.color.rmap.ToABGR(), p.color.gmap.ToABGR(), p.color.bmap.ToABGR());

	const TrailSprite* t2d_spr = VI_DOWNCASTING<const TrailSprite*>(spr);
	t2d_spr->Draw(p);
}

sm::rect TrailSymbol::GetBounding(const Sprite* spr) const
{
	return sm::rect(sm::vec2(0, 0), 200, 200);
}

void TrailSymbol::SetEmitterCfg(t2d_emitter_cfg* cfg)
{
	if (m_et_cfg == cfg) {
		return;
	}

	m_et_cfg = cfg;

	if (m_et) {
		t2d_emitter_release(m_et);
	}
	m_et = t2d_emitter_create(m_et_cfg);
	t2d_emitter_start(m_et);
}

}