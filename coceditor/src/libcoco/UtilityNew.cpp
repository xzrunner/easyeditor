#include "UtilityNew.h"

namespace libcoco
{

bool Utility::IsAnchor(const d2d::ISprite* spr)
{
	const d2d::FontSprite* font = dynamic_cast<const d2d::FontSprite*>(spr);
	return font && font->font.empty() && font->color == d2d::Colorf(0, 0, 0, 0);
}

bool Utility::IsNameValid(const std::string& name)
{
	return !name.empty() && name[0] != '_';
}

}