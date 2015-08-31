#ifndef _EASYICON_QUAD_ICON_H_
#define _EASYICON_QUAD_ICON_H_

#include "Icon.h"

namespace eicon
{

class QuadIcon : public Icon
{
public:
	QuadIcon();
	QuadIcon(const d2d::Vector* src, const d2d::Vector* screen);

	virtual const char* GetIconDesc() const { return "quad"; }

	virtual void Draw(const d2d::Matrix& mt, float process) const;

	virtual void LoadFromFile(const Json::Value& value);
	virtual void StoreToFile(Json::Value& value) const;

	virtual void GetRegion(float process, d2d::Rect& region) const;
	virtual void GetTexcoords4(d2d::Vector tex4[4], float process) const;

private:
	d2d::Vector m_src[4], m_screen[4];

}; // QuadIcon

}

#endif // _EASYICON_QUAD_ICON_H_