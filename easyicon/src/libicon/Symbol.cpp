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

void Symbol::Draw(const d2d::Matrix& mt,
				  const d2d::Colorf& mul, 
				  const d2d::Colorf& add,
				  const d2d::Colorf& r_trans,
				  const d2d::Colorf& g_trans,
				  const d2d::Colorf& b_trans,
				  const d2d::ISprite* sprite/* = NULL*/) const
{
	if (!m_icon) {
		return;
	}

	d2d::ShaderMgr* shader = d2d::ShaderMgr::Instance();
	shader->SetSpriteColor(mul, add);
	shader->SetSpriteColorTrans(r_trans, g_trans, b_trans);

	float process = 1;
	if (sprite) {
		process = static_cast<const Sprite*>(sprite)->GetProcess();
	}
	m_icon->Draw(mt, process);
}

d2d::Rect Symbol::GetSize(const d2d::ISprite* sprite) const
{
	d2d::Rect r;
	if (m_icon) {
		float process = 1;
		if (sprite) {
			process = static_cast<const Sprite*>(sprite)->GetProcess();
		}
		m_icon->GetRegion(process, r);
	}
	return r;
}

void Symbol::SetIcon(Icon* icon)
{
	d2d::obj_assign((d2d::Object*&)m_icon, icon);
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