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

}

void TextboxSymbol::DrawText(const gtxt_label_style& style, const sm::mat4& mt, const s2::Color& mul, const s2::Color& add, const std::string& text, int time, bool richtext) const
{
	std::string t_text = StringHelper::ToUtf8(text);
	GTxt::Instance()->Draw(style, mt, mul, add, t_text, time, richtext);
}

}