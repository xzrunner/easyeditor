#ifndef _GLUE_RENDER_CONTEXT_H_
#define _GLUE_RENDER_CONTEXT_H_

#include "utility/macro.h"

#include <render/render.h>

namespace glue
{

class RenderContext
{
public:
	RID CreateTexture(const uint8_t* data, int width, int height, TEXTURE_FORMAT format);
	void ReleaseTexture(RID id);

	SINGLETON_DECLARATION(RenderContext)
	
}; // RenderContext

}

#endif // _GLUE_RENDER_CONTEXT_H_