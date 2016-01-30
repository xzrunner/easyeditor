#include "Symbol.h"
#include "Sprite.h"
#include "Icon.h"
#include "FileIO.h"

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

void Symbol::ReloadTexture() const
{
	if (m_icon) {
		m_icon->ReloadTexture();
	}
}

void Symbol::Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color, 
				  const d2d::Sprite* spr, const d2d::Sprite* root) const
{
	if (!m_icon) {
		return;
	}

	d2d::ShaderMgr* shader = d2d::ShaderMgr::Instance();
	shader->SetSpriteColor(color);

	float process = 1;
	if (spr) {
		process = static_cast<const Sprite*>(spr)->GetProcess();
	}
	m_icon->Draw(mt, process);
}

d2d::Rect Symbol::GetSize(const d2d::Sprite* sprite) const
{
	d2d::Rect r;
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
	d2d::obj_assign<Icon>(m_icon, icon);
}

void Symbol::SetImage(d2d::Image* img)
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