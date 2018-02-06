#ifndef _EASYICON_ICON_H_
#define _EASYICON_ICON_H_

#include <sprite2/Icon.h>

#include <json/json.h>

namespace s2s { class IconSym; }

namespace eicon
{

class Icon : public virtual s2::Icon
{
public:
	virtual const char* GetIconDesc() const = 0;

	virtual void LoadFromFile(const Json::Value& value) = 0;
	virtual void StoreToFile(Json::Value& value) const = 0;
	virtual void LoadFromFile(const s2s::IconSym& sym) = 0;

}; // Icon

}

#endif // _EASYICON_ICON_H_