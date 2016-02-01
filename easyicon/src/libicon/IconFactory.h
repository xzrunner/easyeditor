#ifndef _EASYICON_ICON_FACTORY_H_
#define _EASYICON_ICON_FACTORY_H_

#include <json/json.h>

namespace eicon
{

class Icon;

class IconFactory
{
public:
	static Icon* CreateIconFromFile(const Json::Value& value);

}; // IconFactory

}

#endif // _EASYICON_ICON_FACTORY_H_