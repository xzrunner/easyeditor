#ifndef _EASYICON_CHANGED_RECT_ICON_H_
#define _EASYICON_CHANGED_RECT_ICON_H_

#include "Icon.h"

#include <sprite2/DynamicRectIcon.h>

namespace eicon
{

class ChangedRectIcon : public Icon, public s2::DynamicRectIcon
{
public:
	/**
	 *  @interface
	 *    Icon
	 */
	virtual const char* GetIconDesc() const override { return "changed-rect"; }

	virtual void LoadFromFile(const Json::Value& value) override;
	virtual void StoreToFile(Json::Value& value) const override;
	virtual void LoadFromFile(const sns::IconSym& sym) override;

}; // ChangedRectIcon

}

#endif // _EASYICON_CHANGED_RECT_ICON_H_