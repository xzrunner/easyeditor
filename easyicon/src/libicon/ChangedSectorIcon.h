#ifndef _EASYICON_SECTOR_ICON_H_
#define _EASYICON_SECTOR_ICON_H_

#include "Icon.h"

#include <sprite2/DynamicSectorIcon.h>

namespace eicon
{

class ChangedSectorIcon : public Icon, public s2::DynamicSectorIcon
{
public:
	virtual const char* GetIconDesc() const { return "changed-sector"; }

	virtual void LoadFromFile(const Json::Value& value);
	virtual void StoreToFile(Json::Value& value) const;

}; // ChangedSectorIcon 

}

#endif // _EASYICON_SECTOR_ICON_H_