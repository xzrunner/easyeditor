#ifndef _SPRITE2_RENDER_CONTEXT_H_
#define _SPRITE2_RENDER_CONTEXT_H_

#include <SM_Vector.h>

namespace s2
{

class RenderCtx
{
public:
	sm::vec2 mv_offset;
	float mv_scale;

	int proj_width;
	int proj_height;

public:
	RenderCtx() 
		: mv_scale(0), proj_width(0), proj_height(0) {}
	RenderCtx(int width, int height)
		: mv_offset(0, 0), mv_scale(1), proj_width(width), proj_height(height) {}

}; // RenderCtx

}

#endif // _SPRITE2_RENDER_CONTEXT_H_