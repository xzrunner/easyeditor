#include "Symbol.h"
#include "Sprite.h"
#include "Icon.h"
#include "FileIO.h"

#include <shaderlab.h>
#include <sprite2/RenderParams.h>

namespace eicon
{

Symbol::Symbol()
	: m_icon(NULL)
{
}

Symbol::~Symbol()
{
	if (m_icon) {
		m_icon->RemoveReference();
	}
}

void Symbol::Draw(const s2::RenderParams& params, const s2::Sprite* spr) const
{
	if (!m_icon) {
		return;
	}

	s2::RenderParams p = params;
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
		process = dynamic_cast<const Sprite*>(spr)->GetProcess();
	}
	m_icon->Draw(p.mt, process);
}

sm::rect Symbol::GetBounding(const s2::Sprite* spr) const
{
	sm::rect r;
	if (!m_icon) {
		return r;
	}

	float process = 1;
	if (spr) {
		process = dynamic_cast<const Sprite*>(spr)->GetProcess();
	}
	return m_icon->GetRegion(process);
}

void Symbol::ReloadTexture() const
{
	if (m_icon) {
		m_icon->ReloadTexture();
	}
}

void Symbol::SetIcon(Icon* icon)
{
	cu::RefCountObjAssign(m_icon, icon);
}

void Symbol::SetImage(ee::Image* img)
{
	if (m_icon) {
		m_icon->SetImage(img);
	}
}

void Symbol::LoadResources()
{
	Icon* icon = FileIO::LoadFromFile(m_filepath.c_str());
	SetIcon(icon);
	icon->RemoveReference();
}

}