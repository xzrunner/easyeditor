#ifndef _SPRITE2_RENDER_SHADER_H_
#define _SPRITE2_RENDER_SHADER_H_

#include "FilterMode.h"
#include "BlendMode.h"

namespace s2
{

class RenderShader
{
public:
	FilterMode	filter;
	BlendMode	blend;

	float filter_params[4];

public:
	RenderShader() 
		: filter(FM_NULL)
		, blend(BM_NULL)
	{}

}; // RenderShader

}

#endif // _SPRITE2_RENDER_SHADER_H_