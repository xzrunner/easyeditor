#include "Utility.h"

namespace librespacker
{

bool Utility::IsAnchor(const d2d::Sprite* spr)
{
	const d2d::FontBlankSprite* font = dynamic_cast<const d2d::FontBlankSprite*>(spr);
	return font && font->font.empty() && font->font_color == d2d::Colorf(0, 0, 0, 0);
}

bool Utility::IsNameValid(const std::string& name)
{
	return !name.empty() && name[0] != '_';
}

}