#ifndef _EASYEDITOR_RENDER_COLOR_H_
#define _EASYEDITOR_RENDER_COLOR_H_

#include "Color.h"

#include <json/json.h>

namespace ee
{

class RenderColor
{
public:
	RenderColor();

	void LoadFromFile(const Json::Value& val);
	void StoreToFile(Json::Value& val) const;

public:
	Colorf multi, add;
	Colorf r, g, b;

}; // RenderColor

}

#endif // _EASYEDITOR_RENDER_COLOR_H_
