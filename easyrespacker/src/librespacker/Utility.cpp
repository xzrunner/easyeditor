#include "Utility.h"

#include <ee/FontBlankSprite.h>

namespace erespacker
{

bool Utility::IsAnchor(const ee::SprConstPtr& spr)
{
	auto font = std::dynamic_pointer_cast<const ee::FontBlankSprite>(spr);
	return font && font->font.empty() && font->font_color == s2::Color(0, 0, 0, 0);
}

bool Utility::IsNameValid(const std::string& name)
{
	return !name.empty() && name[0] != '_';
}

}