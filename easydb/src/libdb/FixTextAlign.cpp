#include "FixTextAlign.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/SymbolFile.h>
#include <ee/EE_GTxt.h>

#include <gtxt_label.h>
#include <gtxt_util.h>
#include <gtxt_freetype.h>
#include <sprite2/SymType.h>
#include <sprite2/Textbox.h>
#include <gum/TextboxLoader.h>

namespace edb
{

std::string FixTextAlign::Command() const
{
	return "fix-text-align";
}

std::string FixTextAlign::Description() const
{
	return "fix text align";
}

std::string FixTextAlign::Usage() const
{
	return Command() + " [dir]";
}

int FixTextAlign::Run(int argc, char *argv[])
{
	// fix-text-align D:\projects\ejoy\coco-tools\sg_particle\data\json_and_pngs

	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	ee::GTxt::Init();

	Trigger(argv[2]);

	return 0;
}

bool FixTextAlign::FixSprite(const std::string& filepath, Json::Value& spr_val) const
{
	std::string child_path = spr_val["filepath"].asString();

	if (child_path.find("text_white_text") != std::string::npos) {
		int zz = 0;
	}

	if (ee::SymbolFile::Instance()->Type(child_path) != s2::SYM_TEXTBOX) {
		return false;
	}
	if (!spr_val.isMember("text")) {
		return false;
	}

	s2::Textbox tb;
	gum::TextboxLoader loader(tb);
	loader.LoadJson(spr_val["text"]);

	if (tb.align_hori != s2::Textbox::HA_CENTER) {
		return false;
	}
	if (!tb.has_edge) {
		return false;
	}

	std::string text = spr_val["text"]["text"].asString();
	if (text.empty()) {
		return false;
	}

	gtxt_glyph_style gs;

	gs.font				= tb.font_type;
	gs.font_size			= tb.font_size;
	gs.font_color.integer = tb.font_color.ToRGBA();

	gs.edge				= tb.has_edge;
	gs.edge_size			= tb.edge_size;
	gs.edge_color.integer = tb.edge_color.ToRGBA();

	int ret = gtxt_unicode_len(text[0]);
	int unicode = gtxt_get_unicode(text.c_str(), ret);

	gtxt_glyph_layout out;
	gtxt_ft_get_layout(unicode, &gs, &out);
	float xoff = out.bearing_x - out.bearing_x_old;
	if (xoff == 0) {
		return false;
	}

	float x = 0, y = 0;
	if (spr_val.isMember("position")) {
		x = spr_val["position"]["x"].asDouble();
		y = spr_val["position"]["y"].asDouble();
	}
	x -= xoff;

	spr_val["position"]["x"] = x;
	spr_val["position"]["y"] = y;

	return true;
}

void FixTextAlign::Trigger(const std::string& dir) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());

		if (filepath.find("yq_cheng_complex") != std::string::npos) {
			int zz = 0;
		}

		int type = ee::SymbolFile::Instance()->Type(filepath);
		switch (type)
		{
		case s2::SYM_COMPLEX:
			FixComplex(filepath);
			break;
		case s2::SYM_ANIMATION:
			FixAnim(filepath);
			break;
		}
	}
}

}