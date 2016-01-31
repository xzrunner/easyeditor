#include "Utility.h"

namespace librespacker
{

bool Utility::IsAnchor(const ee::Sprite* spr)
{
	const ee::FontBlankSprite* font = dynamic_cast<const ee::FontBlankSprite*>(spr);
	return font && font->font.empty() && font->font_color == ee::Colorf(0, 0, 0, 0);
}

bool Utility::IsNameValid(const std::string& name)
{
	return !name.empty() && name[0] != '_';
}

}