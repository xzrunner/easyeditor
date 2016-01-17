#include "RenderContext.h"

#include <sl_shader.h>

#include <stddef.h>

namespace glue
{

SINGLETON_DEFINITION(RenderContext)

RenderContext::RenderContext() 
{
	sl_shader_init();
}

RenderContext::~RenderContext() 
{
}

RID RenderContext::CreateTexture(const uint8_t* data, int width, int height, TEXTURE_FORMAT format)
{
	render* r = sl_shader_get_render();
	return render_texture_create(r, width, height, format, TEXTURE_2D, 0);
}

void RenderContext::ReleaseTexture(RID id)
{
	render* r = sl_shader_get_render();
	render_release(r, TEXTURE, id);
}

}