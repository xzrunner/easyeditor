#ifndef _SPRITE2_DYNAMIC_SECTOR_ICON_H_
#define _SPRITE2_DYNAMIC_SECTOR_ICON_H_

#include "Icon.h"

namespace s2
{

class DynamicSectorIcon : public VIRTUAL_INHERITANCE Icon
{
public:
	DynamicSectorIcon();

	void SetRegion(float min, float max);
	void GetRegion(float& min, float& max) const;

protected:
	virtual void GetQuad(float process, sm::vec2 quad[4]) const;

private:
	float m_min, m_max;

}; // DynamicSectorIcon

}

#endif // _SPRITE2_DYNAMIC_SECTOR_ICON_H_