#include "Symbol.h"
#include "Sprite.h"
#include "GTxt.h"

namespace etext
{

Symbol::Symbol()
{
	m_bg_style.fill = true;
	m_bg_style.color.set(0.7f, 0.7f, 0.7f, 0.7f);
}

Symbol::~Symbol()
{
}

void Symbol::ReloadTexture() const
{
}

void Symbol::Draw(const d2d::Matrix& mt,
				  const d2d::Colorf& mul, 
				  const d2d::Colorf& add,
				  const d2d::Colorf& r_trans,
				  const d2d::Colorf& g_trans,
				  const d2d::Colorf& b_trans,
				  const d2d::ISprite* sprite/* = NULL*/) const
{
	const d2d::SettingData& setting = d2d::Config::Instance()->GetSettings();
	if (setting.visible_label_bg) {
		DrawBackground(sprite, mt);
	} 
 	if (setting.visible_label_text) {
 		DrawText(sprite, mt);
 	}
}

d2d::Rect Symbol::GetSize(const d2d::ISprite* sprite) const
{
	return d2d::Rect(m_width, m_height);
}

void Symbol::LoadResources()
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	m_width = value["width"].asInt();
	m_height = value["height"].asInt();

	m_font_size = value["font size"].asInt();

	m_has_edge = value["edge"].asBool();

	m_color = value["color"].asString();

	m_align_hori = (HoriAlignType)(value["align hori"].asInt());
	m_align_vert = (VertAlignType)(value["align vert"].asInt());
}

void Symbol::DrawBackground(const d2d::ISprite* sprite, const d2d::Matrix& mt) const
{
	if (!sprite) {
		return;
	}

	if (const Sprite* font = dynamic_cast<const Sprite*>(sprite)) {
		int w, h;
		font->GetSize(w, h);
		d2d::PrimitiveDraw::rect(mt, w * 0.5f, h * 0.5f, m_bg_style);
	}
}

void Symbol::DrawText(const d2d::ISprite* sprite, const d2d::Matrix& mt) const
{
	if (!sprite) {
		return;
	}

	if (const Sprite* font = dynamic_cast<const Sprite*>(sprite)) {
		GTxt::Instance()->Draw(mt, font);
	}
}

}