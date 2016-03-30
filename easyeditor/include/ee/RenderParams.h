#ifndef _EASYEDITOR_RENDER_PARAMS_H_
#define _EASYEDITOR_RENDER_PARAMS_H_

#include "Matrix.h"
#include "RenderColor.h"
#include "RenderShader.h"
#include "RenderCamera.h"

namespace ee
{

class RenderParams
{
public:
	RenderParams();
	RenderParams(const RenderParams& trans);
	RenderParams(const Matrix& mt);
	RenderParams(const Matrix& mt, const RenderColor& color);

	void LoadFromFile(const Json::Value& val);
	void StoreToFile(Json::Value& val) const;

public:
	Matrix mt;

	RenderColor color;

	RenderShader shader;

	RenderCamera camera;

}; // RenderParams

}

#endif // _EASYEDITOR_RENDER_PARAMS_H_
