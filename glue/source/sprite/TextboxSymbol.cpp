#include "TextboxSymbol.h"
#include "trans_color.h"
#include "GLUE_GTxt.h"
#include "StringHelper.h"

#include <json/json.h>

#include <fstream>

namespace glue
{

void TextboxSymbol::LoadJson(const std::string& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	m_tb.width			= value["width"].asInt();
	m_tb.height			= value["height"].asInt();

	m_tb.font_type		= value["font"].asInt();
	m_tb.font_size		= value["font_size"].asInt();
	m_tb.font_color		= str2color(value["font_color"].asString(), PT_RGBA);

	m_tb.has_edge		= value["edge"].asBool();
	m_tb.edge_size		= static_cast<float>(value["edge_size"].asDouble());
	m_tb.edge_color		= str2color(value["edge_color"].asString(), PT_RGBA);

	m_tb.space_hori		= static_cast<float>(value["space_hori"].asDouble());
	m_tb.space_vert		= static_cast<float>(value["space_vert"].asDouble());

	m_tb.align_hori		= (s2::Textbox::HoriAlign)(value["align_hori"].asInt());
	m_tb.align_vert		= (s2::Textbox::VertAlign)(value["align_vert"].asInt());

	m_tb.overflow		= true;
	if (!value["overflow"].isNull()) {
		m_tb.overflow	= value["overflow"].asBool();
	}

	m_tb.richtext		= true;
	if (!value["richtext"].isNull()) {
		m_tb.richtext	= value["richtext"].asBool();
	}
}

void TextboxSymbol::DrawText(const gtxt_label_style& style, const sm::mat4& mt, const s2::Color& mul, const s2::Color& add, const std::string& text, int time, bool richtext) const
{
	std::string t_text = StringHelper::ToUtf8(text);
	GTxt::Instance()->Draw(style, mt, mul, add, t_text, time, richtext);
}

}