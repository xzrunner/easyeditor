#ifndef _EASYICON_QUAD_ICON_H_
#define _EASYICON_QUAD_ICON_H_

#include "Icon.h"

namespace eicon
{

class QuadIcon : public Icon
{
public:
	QuadIcon();
	QuadIcon(d2d::Image* img, const d2d::Vector* src, 
		const d2d::Vector* screen);

	virtual const char* GetIconDesc() const { return "quad"; }

	virtual void LoadFromFile(const Json::Value& value);
	virtual void StoreToFile(Json::Value& value) const;

	virtual void GetBound(float process, d2d::Vector bound[4]) const;

	virtual void Draw(const d2d::Matrix& mt, float process) const;

	virtual d2d::Rect GetRegion(float process) const;

private:
	d2d::Vector m_src[4], m_screen[4];

}; // QuadIcon

}

#endif // _EASYICON_QUAD_ICON_H_