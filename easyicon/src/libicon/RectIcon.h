#ifndef _EASYICON_RECT_ICON_H_
#define _EASYICON_RECT_ICON_H_

#include "Icon.h"

namespace eicon
{

class RectIcon : public Icon
{
public:
	RectIcon();

	virtual const char* GetIconDesc() const { return "rect"; }

	virtual void LoadFromFile(const Json::Value& value);
	virtual void StoreToFile(Json::Value& value) const;

	virtual void GetBound(float process, d2d::Vector bound[4]) const;

	void SetRegion(const d2d::Rect& r);

private:
	d2d::Vector m_min, m_max;

}; // RectIcon

}

#endif // _EASYICON_RECT_ICON_H_