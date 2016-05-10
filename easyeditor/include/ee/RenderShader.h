#ifndef _EASYEDITOR_RENDER_SHADER_H_
#define _EASYEDITOR_RENDER_SHADER_H_

#include <sprite2/RenderShader.h>

#include <json/json.h>

namespace ee
{

class RenderShader : public s2::RenderShader
{
public:
	void LoadFromFile(const Json::Value& val);
	void StoreToFile(Json::Value& val) const;

}; // RenderShader

}

#endif // _EASYEDITOR_RENDER_SHADER_H_
