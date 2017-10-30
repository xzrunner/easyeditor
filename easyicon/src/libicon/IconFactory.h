#ifndef _EASYICON_ICON_FACTORY_H_
#define _EASYICON_ICON_FACTORY_H_

#include <json/json.h>

#include <memory>

namespace s2 { class Icon; }

namespace eicon
{

class IconFactory
{
public:
	static std::unique_ptr<s2::Icon> CreateIconFromFile(const Json::Value& value);

}; // IconFactory

}

#endif // _EASYICON_ICON_FACTORY_H_