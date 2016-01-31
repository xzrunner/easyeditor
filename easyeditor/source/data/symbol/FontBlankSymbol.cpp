#include "FontBlankSymbol.h"
#include "FontBlankSprite.h"
#include "Vector.h"
#include "trans_color.h"
#include "FileType.h"
#include "FileHelper.h"
#include "Config.h"
#include "SettingData.h"
#include "StringHelper.h"
#include "PrimitiveDraw.h"

#include <json/json.h>

#include <fstream>

namespace ee
{

FontBlankSymbol::FontBlankSymbol()
{
	static int id = 0;
	m_name = std::string("fontblank") + StringHelper::ToString(id++);

	align_hori = align_vert = 0;
	size = 0;
	width = height = 100;

	m_style.fill = true;
	m_style.color.Set(0.7f, 0.7f, 0.7f, 0.7f);
}

FontBlankSymbol::~FontBlankSymbol()
{
}

void FontBlankSymbol::ReloadTexture() const
{
}

void FontBlankSymbol::Draw(const Matrix& mt, const ColorTrans& col, 
						   const Sprite* spr, const Sprite* root) const
{
	const SettingData& setting = Config::Instance()->GetSettings();
	const FontBlankSprite* fb = (const FontBlankSprite*)spr;
	if (setting.visible_label_bg) {
 		DrawBackground(fb, mt);
 	} 
	if (setting.visible_label_text) {
		DrawText(fb, mt);
	}
}

Rect FontBlankSymbol::GetSize(const Sprite* sprite/* = NULL*/) const
{
	float w = width, h = height;
	if (sprite) {
		const FontBlankSprite* font = dynamic_cast<const FontBlankSprite*>(sprite);
		if (font) {
			w = font->width;
			h = font->height;
		}
	}
	return Rect(w, h);
}

bool FontBlankSymbol::LoadFont(const std::string& _filename)
{
	if (_filename == filename)
		return false;

	std::string filename = FileHelper::GetAbsolutePath(m_filepath, _filename);
	if (!FileType::IsType(filename, FileType::e_freetype) || 
		!FileHelper::IsFileExist(filename)) {
		return false;
	}

	return false;
}

void FontBlankSymbol::LoadResources()
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	name = value["name"].asString();
	font = value["font"].asString();
	color = value["color"].asString();
	if (!value["align"].isNull()) {
		align_hori = value["align"].asDouble();
	} else {
		align_hori = value["align hori"].asDouble();
		align_vert = value["align vert"].asDouble();
	}
	size = value["size"].asDouble();
	width = value["width"].asDouble();
	height = value["height"].asDouble();
	if (value["font filename"].isNull())
		;
	else {
		filename = value["font filename"].asString();
		LoadFont(filename);
	}
}

void FontBlankSymbol::DrawBackground(const FontBlankSprite* fb, const Matrix& mt) const
{
	float hw = width * 0.5f,
		  hh = height * 0.5f;
	if (fb) {
		hw = fb->width * 0.5f;
		hh = fb->height * 0.5f;
	}

	PrimitiveDraw::DrawRect(mt, hw, hh, m_style);
}

void FontBlankSymbol::DrawText(const FontBlankSprite* fb, const Matrix& mt) const
{
// 	if (sprite) {
// 		if (const FontBlankSprite* font = dynamic_cast<const FontBlankSprite*>(sprite)) {
// 			const std::string& str = font->GetTextContext();
// 			if (!str.empty()) {
// 				Vector pos = Math2D::TransVector(Vector(0, 0), mt);
// 
// 				LabelStyle style;
// 				style.has_edge = font->has_edge;
// 				style.font_size = font->size;
// 				style.width = font->width;
// 				style.height = font->height;
// 				style.color = font->color;
// 				style.align_hori = font->align_hori;
// 				style.align_vert = font->align_vert;
// 
// 				LabelNew::Print(str.utf8_str(), pos, 1, style);
// 			}
// 		}
// 	}

// 	if (sprite) {
// 		if (const FontBlankSprite* font = dynamic_cast<const FontBlankSprite*>(sprite)) {
// 			const std::string& str = font->GetTextContext();
// 			if (!str.empty()) {
// 				PrimitiveDraw::text(str.c_str());
// 			}
// 		}
// 	}

// 	if (m_font) 
// 	{
// 		float w = width;
// 		if (const FontBlankSprite* s = dynamic_cast<const FontBlankSprite*>(sprite)) {
// 			w = s->width;
// 		}
// 		Shader::Instance()->null();
// 		print(-w*0.5f, 0, "abcdEFGH1234");
// 	}
}

}