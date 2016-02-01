#include "IconFactory.h"
#include "IconType.h"
#include "RectIcon.h"
#include "QuadIcon.h"
#include "ChangedRectIcon.h"
#include "ChangedSectorIcon.h"

namespace eicon
{

Icon* IconFactory::CreateIconFromFile(const Json::Value& value)
{
	Icon* icon = NULL;

	std::string desc = value["type"].asString();
	IconType type = get_icon_type(desc);
	switch (type)
	{
	case IT_RECT:
		icon = new RectIcon;
		break;
	case IT_QUAD:
		icon = new QuadIcon;
		break;
	case IT_CHANGED_RECT:
		icon = new ChangedRectIcon;
		break;
	case IT_CHANGED_SECTOR:
		icon = new ChangedSectorIcon;
		break;
	}

	return icon;
}

}