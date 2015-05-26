#ifndef _EASYICON_SECTOR_ICON_H_
#define _EASYICON_SECTOR_ICON_H_

#include "Icon.h"

namespace eicon
{

class ChangedSectorIcon : public Icon
{
public:
	ChangedSectorIcon();

	virtual const char* GetIconDesc() const { return "changed-sector"; }

	virtual void Draw(const d2d::Matrix& mt, float process) const;

	virtual void LoadFromFile(const Json::Value& value);
	virtual void StoreToFile(Json::Value& value) const;

	virtual void GetRegion(float process, d2d::Rect& region) const;
	virtual void GetTexcoords4(d2d::Vector tex4[4], float process) const;

	void SetRegion(float angle_min, float angle_max);

private:
	float m_min, m_max;

}; // ChangedSectorIcon 

}

#endif // _EASYICON_SECTOR_ICON_H_