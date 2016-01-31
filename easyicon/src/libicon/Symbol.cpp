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

void Symbol::Draw(const ee::Matrix& mt, const ee::ColorTrans& color, 
				  const ee::Sprite* spr, const ee::Sprite* root) const
{
	if (!m_icon) {
		return;
	}

	ee::ShaderMgr* shader = ee::ShaderMgr::Instance();
	shader->SetSpriteColor(color);

	float process = 1;
	if (spr) {
		process = static_cast<const Sprite*>(spr)->GetProcess();
	}
	m_icon->Draw(mt, process);
}

ee::Rect Symbol::GetSize(const ee::Sprite* sprite) const
{
	ee::Rect r;
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