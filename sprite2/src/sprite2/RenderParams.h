#ifndef _SPRITE2_RENDER_PARAMS_H_
#define _SPRITE2_RENDER_PARAMS_H_

#include "Matrix.h"
#include "RenderColor.h"
#include "RenderShader.h"
#include "RenderCamera.h"

namespace s2
{

class Sprite;

class RenderParams
{
public:
	Sprite* root_spr;

	bool set_shader;

	Matrix mat;

	RenderColor	color;
	
	RenderShader shader;

	RenderCamera camera;
	
public:
	RenderParams();

}; // RenderStyle

}

#include "RenderParams.inl"

#endif // _SPRITE2_RENDER_PARAMS_H_