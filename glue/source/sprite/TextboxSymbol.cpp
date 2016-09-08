#include "TextboxSymbol.h"
#include "TextboxLoader.h"
#include "GLUE_GTxt.h"
#include "StringHelper.h"

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

	TextboxLoader loader(GetTextbox());
	loader.LoadJson(value);
}

void TextboxSymbol::DrawText(const gtxt_label_style& style, const sm::mat4& mt, const s2::Color& mul, const s2::Color& add, const std::string& text, int time, bool richtext) const
{
	std::string t_text = StringHelper::ToUtf8(text);
	GTxt::Instance()->Draw(style, mt, mul, add, t_text, time, richtext);
}

}