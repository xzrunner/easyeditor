#ifndef _EASYEDITOR_COLOR_TRANS_H_
#define _EASYEDITOR_COLOR_TRANS_H_

#include "Color.h"

#include <json/json.h>

namespace ee
{

class ColorTrans
{
public:
	ColorTrans();

	void LoadFromFile(const Json::Value& val);
	void StoreToFile(Json::Value& val) const;

public:
	Colorf multi, add;
	Colorf r, g, b;

}; // ColorTrans

}

#endif // _EASYEDITOR_COLOR_TRANS_H_
