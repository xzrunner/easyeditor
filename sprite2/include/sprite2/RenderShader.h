#ifndef _SPRITE2_RENDER_SHADER_H_
#define _SPRITE2_RENDER_SHADER_H_

#include "BlendMode.h"
#include "FastBlendMode.h"

#include <stddef.h>

namespace s2
{

class RenderFilter;

class RenderShader
{
public:
	RenderFilter*	filter;
	BlendMode		blend;
	FastBlendMode	fast_blend;

public:
	RenderShader();
	~RenderShader();

	RenderShader operator * (const RenderShader& rs) const;

}; // RenderShader

}

#endif // _SPRITE2_RENDER_SHADER_H_