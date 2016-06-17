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
		m_icon->Release();
	}
}

void Symbol::Draw(const s2::RenderParams& params, const ee::Sprite* spr) const
{
	if (!m_icon) {
		return;
	}

	s2::RenderParams p = params;
	if (spr) {
		p.mt = spr->GetTransMatrix() * params.mt;
		p.color = spr->GetColor() * params.color;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(p.color.mul.ToABGR(), p.color.add.ToABGR());
	shader->SetColorMap(p.color.rmap.ToABGR(), p.color.gmap.ToABGR(), p.color.bmap.ToABGR());

	float process = 1;
	if (spr) {
		process = static_cast<const Sprite*>(spr)->GetProcess();
	}
	m_icon->Draw(p.mt, process);
}

void Symbol::ReloadTexture() const
{
	if (m_icon) {
		m_icon->ReloadTexture();
	}
}

sm::rect Symbol::GetSize(const ee::Sprite* sprite) const
{
	sm::rect r;
	if (!m_icon) {
		return r;
	}

	float process = 1;
	if (sprite) {
		process = static_cast<const Sprite*>(sprite)->GetProcess();
	}
	return m_icon->GetRegion(process);
}

void Symbol::SetIcon(Icon* icon)
{
	ee::obj_assign<Icon>(m_icon, icon);
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
	icon->Release();
}

}