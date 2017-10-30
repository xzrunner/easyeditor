#ifndef _EASYEDITOR_HISTORY_UTIL_H_
#define _EASYEDITOR_HISTORY_UTIL_H_

#include "Sprite.h"

#include <json/json.h>

namespace ee
{

class HistoryUtil
{
public:
	static Json::Value StoreSpritesIndex(const std::vector<SprPtr>& myself,
		const std::vector<SprPtr>& other);

}; // HistoryUtil

}

#endif // _EASYEDITOR_HISTORY_UTIL_H_