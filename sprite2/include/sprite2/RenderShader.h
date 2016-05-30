#ifndef _SPRITE2_RENDER_SHADER_H_
#define _SPRITE2_RENDER_SHADER_H_

#include "BlendMode.h"

#include <stddef.h>

namespace s2
{

class RenderFilter;

class RenderShader
{
public:
	RenderFilter*	filter;
	BlendMode		blend;

	float filter_params[4];

public:
	RenderShader()
		: filter(NULL)
		, blend(BM_NULL)
	{}

}; // RenderShader

}

#endif // _SPRITE2_RENDER_SHADER_H_