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

	virtual void LoadFromFile(const Json::Value& value);
	virtual void StoreToFile(Json::Value& value) const;

	virtual void GetBound(float process, ee::Vector bound[4]) const;

private:
	float m_min, m_max;

}; // ChangedSectorIcon 

}

#endif // _EASYICON_SECTOR_ICON_H_