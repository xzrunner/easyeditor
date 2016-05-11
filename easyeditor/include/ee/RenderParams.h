#ifndef _EASYEDITOR_RENDER_PARAMS_H_
#define _EASYEDITOR_RENDER_PARAMS_H_

#include <SM_Matrix.h>
#include <sprite2/RenderColor.h>
#include <sprite2/RenderShader.h>
#include <sprite2/RenderCamera.h>

#include <json/json.h>

namespace ee
{

class RenderParams
{
public:
	RenderParams();
	RenderParams(const RenderParams& trans);
	RenderParams(const sm::mat4& mt);
	RenderParams(const sm::mat4& mt, const s2::RenderColor& color);

	void LoadFromFile(const Json::Value& val);
	void StoreToFile(Json::Value& val) const;

public:
	bool set_shader;

	sm::mat4 mt;

	s2::RenderColor color;

	s2::RenderShader shader;

	s2::RenderCamera camera;

}; // RenderParams

}

#endif // _EASYEDITOR_RENDER_PARAMS_H_
