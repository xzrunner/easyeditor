#ifndef _EASYICON_RECT_ICON_H_
#define _EASYICON_RECT_ICON_H_

#include "Icon.h"

#include <sprite2/StaticRectIcon.h>

#include <json/json.h>

namespace eicon
{

class RectIcon : public Icon, public s2::StaticRectIcon
{
public:
	/**
	 *  @interface
	 *    Icon
	 */
	virtual const char* GetIconDesc() const override { return "rect"; }

	virtual void LoadFromFile(const Json::Value& value) override;
	virtual void StoreToFile(Json::Value& value) const override;
	virtual void LoadFromFile(const s2s::IconSym& sym) override;

}; // RectIcon

}

#endif // _EASYICON_RECT_ICON_H_