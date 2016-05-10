#ifndef _EASYEDITOR_RENDER_COLOR_H_
#define _EASYEDITOR_RENDER_COLOR_H_

#include <sprite2/RenderColor.h>

#include <json/json.h>

namespace ee
{

class RenderColor : public s2::RenderColor
{
public:
	void LoadFromFile(const Json::Value& val);
	void StoreToFile(Json::Value& val) const;

}; // RenderColor

}

#endif // _EASYEDITOR_RENDER_COLOR_H_
