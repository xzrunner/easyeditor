#ifndef _SPRITE2_RENDER_PARAMS_H_
#define _SPRITE2_RENDER_PARAMS_H_

//#include "Matrix.h"
#include "RenderColor.h"
#include "RenderShader.h"
#include "RenderCamera.h"

#include <SM_Matrix.h>

namespace s2
{

class Sprite;

class RenderParams
{
public:
	const Sprite* root_spr;	// for blend

	bool set_shader;

//	Matrix mt;
	sm::mat4 mt;

	RenderColor	color;
	
	RenderShader shader;

	s2::RenderCamera camera;
	
public:
	RenderParams();

}; // RenderStyle

}

#include "RenderParams.inl"

#endif // _SPRITE2_RENDER_PARAMS_H_