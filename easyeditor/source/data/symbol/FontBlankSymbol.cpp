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
#include <sprite2/RVG.h>
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

s2::RenderReturn FontBlankSymbol::DrawTree(cooking::DisplayList* dlist, const s2::RenderParams& rp, const s2::Sprite* spr) const
{
	s2::RenderParams p = rp;
	if (spr) {
		p.mt = spr->GetLocalMat() * rp.mt;
		p.col_common = spr->GetColorCommon() * rp.col_common;
		p.col_map    = spr->GetColorMap() * rp.col_map;
	}
	const SettingData& setting = Config::Instance()->GetSettings();
	const FontBlankSprite* fb = dynamic_cast<const FontBlankSprite*>(spr);
	if (setting.visible_label_bg) {
 		DrawBackground(fb, p.mt);
 	} 
	if (setting.visible_label_text) {
		DrawText(fb, p.mt);
	}
	return s2::RENDER_OK;
}

sm::rect FontBlankSymbol::GetBoundingImpl(const s2::Sprite* spr, const s2::Actor* actor, bool cache) const
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
	if (!gum::FilepathHelper::Exists(m_filepath.c_str())) {
		return false;
	}

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	name = value["name"].asString().c_str();
	font = value["font"].asString().c_str();
	color = value["color"].asString().c_str();
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
		filename = value["font filename"].asString().c_str();
		LoadFont(filename);
	}

	return true;
}

void FontBlankSymbol::DrawBackground(const FontBlankSprite* fb, const S2_MAT& mt) const
{
	s2::RVG::SetColor(pt2::Color(179, 179, 179, 179));

	float hw = width * 0.5f,
		  hh = height * 0.5f;
	if (fb) {
		hw = fb->width * 0.5f;
		hh = fb->height * 0.5f;
	}

	sm::vec2 min(-hw, -hh), max(hw, hh);
	min = mt * min;
	max = mt * max;

	s2::RVG::Rect(nullptr, min, max, true);
}

void FontBlankSymbol::DrawText(const FontBlankSprite* fb, const S2_MAT& mt) const
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