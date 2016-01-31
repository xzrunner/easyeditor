#ifndef _EASYEDITOR_HISTORY_UTIL_H_
#define _EASYEDITOR_HISTORY_UTIL_H_

#include <json/json.h>

namespace ee
{

class Sprite;

class HistoryUtil
{
public:
	static Json::Value StoreSpritesIndex(const std::vector<Sprite*>& myself, 
		const std::vector<Sprite*>& other);

}; // HistoryUtil

}

#endif // _EASYEDITOR_HISTORY_UTIL_H_