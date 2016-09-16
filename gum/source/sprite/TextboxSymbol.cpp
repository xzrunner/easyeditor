#include "TextboxSymbol.h"
#include "GUM_GTxt.h"
#include "StringHelper.h"

namespace gum
{

void TextboxSymbol::DrawText(const gtxt_label_style& style, const sm::mat4& mt, const s2::Color& mul, const s2::Color& add, const std::string& text, int time, bool richtext) const
{
	std::string t_text = StringHelper::ToUtf8(text);
	GTxt::Instance()->Draw(style, mt, mul, add, t_text, time, richtext);
}

}