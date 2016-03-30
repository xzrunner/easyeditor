#ifndef _EASYEDITOR_RENDER_SHADER_H_
#define _EASYEDITOR_RENDER_SHADER_H_

#include "BlendModes.h"
#include "FilterModes.h"

#include <json/json.h>

namespace ee
{

class RenderShader
{
public:
	RenderShader();

	void LoadFromFile(const Json::Value& val);
	void StoreToFile(Json::Value& val) const;

public:
	BlendMode blend;
	FilterMode filter;

}; // RenderShader

}

#endif // _EASYEDITOR_RENDER_SHADER_H_
