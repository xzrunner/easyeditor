#ifndef _DRAG2D_HISTORY_UTIL_H_
#define _DRAG2D_HISTORY_UTIL_H_

#include <json/json.h>

namespace d2d
{

class Sprite;

class HistoryUtil
{
public:
	static Json::Value StoreSpritesIndex(const std::vector<Sprite*>& myself, 
		const std::vector<Sprite*>& other);

}; // HistoryUtil

}

#endif // _DRAG2D_HISTORY_UTIL_H_