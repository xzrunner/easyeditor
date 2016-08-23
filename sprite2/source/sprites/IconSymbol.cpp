#include "IconSymbol.h"
#include "IconSprite.h"
#include "Icon.h"
#include "RenderParams.h"

#include <shaderlab.h>

namespace s2
{

IconSymbol::IconSymbol()
	: m_icon(NULL)
{
}

IconSymbol::~IconSymbol()
{
	if (m_icon) {
		m_icon->RemoveReference();
	}
}

void IconSymbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	if (!m_icon) {
		return;
	}

	RenderParams p = params;
	if (spr) {
		p.mt = spr->GetTransMatrix() * params.mt;
		p.color = spr->Color() * params.color;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(p.color.mul.ToABGR(), p.color.add.ToABGR());
	shader->SetColorMap(p.color.rmap.ToABGR(), p.color.gmap.ToABGR(), p.color.bmap.ToABGR());

	float process = 1;
	if (spr) {
		process = VI_DOWNCASTING<const IconSprite*>(spr)->GetProcess();
	}
	m_icon->Draw(p, process);
}

sm::rect IconSymbol::GetBounding(const Sprite* spr) const
{
	sm::rect r;
	if (!m_icon) {
		return r;
	}

	float process = 1;
	if (spr) {
		process = VI_DOWNCASTING<const IconSprite*>(spr)->GetProcess();
	}
	return m_icon->GetRegion(process);
}

void IconSymbol::SetIcon(Icon* icon)
{
	cu::RefCountObjAssign(m_icon, icon);
}

}