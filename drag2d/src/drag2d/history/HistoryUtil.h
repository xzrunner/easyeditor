#ifndef _DRAG2D_HISTORY_UTIL_H_
#define _DRAG2D_HISTORY_UTIL_H_

#include <json.h>

namespace d2d
{

class ISprite;

class HistoryUtil
{
public:
	static Json::Value StoreSpritesIndex(const std::vector<ISprite*>& myself, 
		const std::vector<ISprite*>& other);

}; // HistoryUtil

}

#endif // _DRAG2D_HISTORY_UTIL_H_