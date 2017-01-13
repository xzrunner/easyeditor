#include "FontBlankSymbol.h"
#include "FontBlankSprite.h"
#include "SymbolFile.h"
#include "FileHelper.h"
#include "Config.h"
#include "SettingData.h"
#include "StringHelper.h"
#include "Math2D.h"
#include "SymbolType.h"

#include <sprite2/RenderParams.h>
#include <sprite2/S2_RVG.h>
#include <gum/FilepathHelper.h>

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
}

FontBlankSymbol::~FontBlankSymbol()
{
}

int FontBlankSymbol::Type() const
{
	return SYM_FONTBLANK;
}

void FontBlankSymbol::Draw(const s2::RenderParams& params, const s2::Sprite* spr) const
{
	s2::RenderParams p = params;
	if (spr) {
		p.mt = spr->GetLocalMat() * params.mt;
		p.color = spr->GetColor() * params.color;
	}
	const SettingData& setting = Config::Instance()->GetSettings();
	const FontBlankSprite* fb = dynamic_cast<const FontBlankSprite*>(spr);
	if (setting.visible_label_bg) {
 		DrawBackground(fb, p.mt);
 	} 
	if (setting.visible_label_text) {
		DrawText(fb, p.mt);
	}
}

sm::rect FontBlankSymbol::GetSize(const Sprite* spr/* = NULL*/) const
{
	sm::rect b;
	float w = width, h = height;
	if (spr) {
		const FontBlankSprite* font = dynamic_cast<const FontBlankSprite*>(spr);
		if (font) {
			w = font->width;
			h = font->height;
		}
	}
	b.Build(w, h);
	return b;
}

bool FontBlankSymbol::LoadFont(const std::string& _filename)
{
	if (_filename == filename)
		return false;

	std::string filename = FileHelper::GetAbsolutePath(m_filepath, _filename);
	if (ee::SymbolFile::Instance()->Type(filename) != ee::SYM_FREETYPE || 
		!FileHelper::IsFileExist(filename)) {
		return false;
	}

	return false;
}

bool FontBlankSymbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath)) {
		return false;
	}

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

	return true;
}

void FontBlankSymbol::DrawBackground(const FontBlankSprite* fb, const sm::mat4& mt) const
{
	s2::RVG::SetColor(s2::Color(179, 179, 179, 179));

	float hw = width * 0.5f,
		  hh = height * 0.5f;
	if (fb) {
		hw = fb->width * 0.5f;
		hh = fb->height * 0.5f;
	}

	sm::vec2 min(-hw, -hh), max(hw, hh);
	min = mt * min;
	max = mt * max;

	s2::RVG::Rect(min, max, true);
}

void FontBlankSymbol::DrawText(const FontBlankSprite* fb, const sm::mat4& mt) const
{
// 	if (spr) {
// 		if (const FontBlankSprite* font = dynamic_cast<const FontBlankSprite*>(spr)) {
// 			const std::string& str = font->GetTextContext();
// 			if (!str.empty()) {
// 				sm::vec2 pos = mt * sm::vec2(0, 0);
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

// 	if (spr) {
// 		if (const FontBlankSprite* font = dynamic_cast<const FontBlankSprite*>(spr)) {
// 			const std::string& str = font->GetTextContext();
// 			if (!str.empty()) {
// 				PrimitiveDraw::text(str.c_str());
// 			}
// 		}
// 	}

// 	if (m_font) 
// 	{
// 		float w = width;
// 		if (const FontBlankSprite* s = dynamic_cast<const FontBlankSprite*>(spr)) {
// 			w = s->width;
// 		}
// 		Shader::Instance()->null();
// 		print(-w*0.5f, 0, "abcdEFGH1234");
// 	}
}

}