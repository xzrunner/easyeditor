#ifndef _EASYEDITOR_RENDER_PARAMS_H_
#define _EASYEDITOR_RENDER_PARAMS_H_

#include "RenderColor.h"
#include "RenderShader.h"
#include "RenderCamera.h"

#include <SM_Matrix.h>

namespace ee
{

class RenderParams
{
public:
	RenderParams();
	RenderParams(const RenderParams& trans);
	RenderParams(const sm::mat4& mt);
	RenderParams(const sm::mat4& mt, const RenderColor& color);

	void LoadFromFile(const Json::Value& val);
	void StoreToFile(Json::Value& val) const;

public:
	bool set_shader;

	sm::mat4 mt;

	RenderColor color;

	RenderShader shader;

	RenderCamera camera;

}; // RenderParams

}

#endif // _EASYEDITOR_RENDER_PARAMS_H_
