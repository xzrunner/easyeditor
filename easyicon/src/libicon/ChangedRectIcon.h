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

	virtual void Draw(const d2d::Matrix& mt, float process) const;

	virtual void LoadFromFile(const Json::Value& value);
	virtual void StoreToFile(Json::Value& value) const;

	virtual void GetRegion(float process, d2d::Rect& region) const;
	virtual void GetTexcoords4(d2d::Vector tex4[4], float process) const;

private:
	d2d::Rect GetCurrRect(float process) const;

private:
	d2d::Rect m_begin, m_end;

}; // ChangedRectIcon

}

#endif // _EASYICON_CHANGED_RECT_ICON_H_