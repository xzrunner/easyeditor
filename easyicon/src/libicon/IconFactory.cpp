#include "IconFactory.h"
#include "IconType.h"

#include "RectIcon.h"
#include "ChangedRectIcon.h"

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
	case IT_CHANGED_RECT:
		icon = new ChangedRectIcon;
		break;
	}

	return icon;
}

}