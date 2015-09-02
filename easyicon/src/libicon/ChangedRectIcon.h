#ifndef _EASYICON_CHANGED_RECT_ICON_H_
#define _EASYICON_CHANGED_RECT_ICON_H_

#include "Icon.h"

namespace eicon
{

class ChangedRectIcon : public Icon
{
public:
	ChangedRectIcon();

	virtual const char* GetIconDesc() const { return "changed-rect"; }

	virtual void LoadFromFile(const Json::Value& value);
	virtual void StoreToFile(Json::Value& value) const;

	virtual void GetBound(float process, d2d::Vector bound[4]) const;

private:
	d2d::Rect m_begin, m_end;

}; // ChangedRectIcon

}

#endif // _EASYICON_CHANGED_RECT_ICON_H_