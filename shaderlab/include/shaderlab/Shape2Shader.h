#ifndef _SHADERLAB_SHAPE2_SHADER_H_
#define _SHADERLAB_SHAPE2_SHADER_H_

#include "ShapeShader.h"

namespace sl
{

class Shape2Shader : public ShapeShader
{
public:
	Shape2Shader(ur::RenderContext* rc);	

	void Draw(const float* positions, int count) const;
	void Draw(const float* positions, const uint32_t* colors, int count) const;
	void Draw(float x, float y, bool dummy) const;

protected:
	virtual void InitMVP(ObserverMVP* mvp) const;

}; // Shape2Shader

}

#endif // _SHADERLAB_SHAPE2_SHADER_H_