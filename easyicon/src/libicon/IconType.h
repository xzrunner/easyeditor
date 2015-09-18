#ifndef _EASYICON_ICON_TYPE_H_
#define _EASYICON_ICON_TYPE_H_

#include <string>

namespace eicon
{

enum IconType
{
	IT_UNKNOWN = 0,
	IT_RECT,
	IT_QUAD,
	IT_CHANGED_RECT,
	IT_CHANGED_SECTOR,

}; // IconType

IconType get_icon_type(const std::string& desc);

}

#endif // _EASYICON_ICON_TYPE_H_