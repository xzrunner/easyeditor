#include "TextboxSymLoader.h"

#include <sprite2/Textbox.h>
#include <sprite2/TextboxSymbol.h>

#include <fstream>

namespace glue
{

TextboxSymLoader::TextboxSymLoader(s2::TextboxSymbol* sym)
	: m_sym(sym)
{
	if (m_sym) {
		m_sym->AddReference();
	}
}

TextboxSymLoader::~TextboxSymLoader()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}
}

void TextboxSymLoader::LoadJson(const std::string& filepath)
{
	//if (!m_sym) {
	//	return;
	//}

	//Json::Value value;
	//Json::Reader reader;
	//std::locale::global(std::locale(""));
	//std::ifstream fin(filepath.c_str());
	//std::locale::global(std::locale("C"));
	//reader.parse(fin, value);
	//fin.close();

	//s2::Textbox& tb = m_sym->GetTextbox();

	//tb.width			= value["width"].asInt();
	//tb.height			= value["height"].asInt();

	//tb.font_type		= value["font"].asInt();
	//tb.font_size		= value["font_size"].asInt();
	//tb.font_color		= str2color(value["font_color"].asString(), PT_RGBA);

	//tb.has_edge			= value["edge"].asBool();
	//tb.edge_size		= static_cast<float>(value["edge_size"].asDouble());
	//tb.edge_color		= str2color(value["edge_color"].asString(), PT_RGBA);

	//tb.space_hori		= static_cast<float>(value["space_hori"].asDouble());
	//tb.space_vert		= static_cast<float>(value["space_vert"].asDouble());

	//tb.align_hori		= (s2::Textbox::HoriAlign)(value["align_hori"].asInt());
	//tb.align_vert		= (s2::Textbox::VertAlign)(value["align_vert"].asInt());

	//tb.overflow			= true;
	//if (!value["overflow"].isNull()) {
	//	tb.overflow		= value["overflow"].asBool();
	//}

	//tb.richtext			= true;
	//if (!value["richtext"].isNull()) {
	//	tb.richtext		= value["richtext"].asBool();
	//}
}

}