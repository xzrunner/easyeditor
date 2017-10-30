#include "IconFactory.h"
#include "IconType.h"
#include "RectIcon.h"
#include "QuadIcon.h"
#include "ChangedRectIcon.h"
#include "ChangedSectorIcon.h"

namespace eicon
{

std::unique_ptr<s2::Icon> IconFactory::CreateIconFromFile(const Json::Value& value)
{
	std::unique_ptr<s2::Icon> icon = nullptr;

	std::string desc = value["type"].asString();
	IconType type = get_icon_type(desc);
	switch (type)
	{
	case IT_RECT:
		icon = std::make_unique<RectIcon>();
		break;
	case IT_QUAD:
		icon = std::make_unique<QuadIcon>();
		break;
	case IT_CHANGED_RECT:
		icon = std::make_unique<ChangedRectIcon>();
		break;
	case IT_CHANGED_SECTOR:
		icon = std::make_unique<ChangedSectorIcon>();
		break;
	}

	return icon;
}

}