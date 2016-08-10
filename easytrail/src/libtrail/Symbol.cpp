#include "Symbol.h"
#include "Sprite.h"
#include "MTConfigMgr.h"
#include "TrailNode.h"

#include <mt_2d.h>
#include <sprite2/RenderParams.h>
#include <shaderlab.h>

namespace etrail
{

Symbol::Symbol()
	: m_et_cfg(NULL)
	, m_et(NULL)
{
	TrailNode::Instance();
}

Symbol::Symbol(const Symbol& s)
	: ee::Symbol(s)
	, m_et_cfg(s.m_et_cfg)
	, m_et(NULL)
{
	TrailNode::Instance();
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

	s2::RenderParams p = params;
	p.color = spr->GetColor() * params.color;

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(p.color.mul.ToABGR(), p.color.add.ToABGR());
	shader->SetColorMap(p.color.rmap.ToABGR(), p.color.gmap.ToABGR(), p.color.bmap.ToABGR());

	Sprite* t2d_spr = const_cast<Sprite*>(static_cast<const Sprite*>(spr));
	t2d_spr->Draw(p);
}

sm::rect Symbol::GetSize(const ee::Sprite* sprite) const
{
	return sm::rect(sm::vec2(0, 0), 200, 200);
}

void Symbol::LoadResources()
{
	m_et_cfg = MTConfigMgr::Instance()->GetConfig(m_filepath);
	m_et = t2d_emitter_create(m_et_cfg);

	t2d_emitter_start(m_et);
}

}