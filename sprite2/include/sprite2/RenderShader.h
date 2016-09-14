#ifndef _SPRITE2_RENDER_SHADER_H_
#define _SPRITE2_RENDER_SHADER_H_

#include "BlendMode.h"
#include "FastBlendMode.h"
#include "FilterMode.h"

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
	RenderShader(const RenderShader& rs);
	RenderShader& operator = (const RenderShader& rs);
	~RenderShader();

	RenderShader operator * (const RenderShader& rs) const;

	void SetFilter(FilterMode mode);

}; // RenderShader

}

#endif // _SPRITE2_RENDER_SHADER_H_