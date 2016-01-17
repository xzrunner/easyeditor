#include "RenderContext.h"

#include <sl_shader.h>
#include <render/render.h>

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
	RID id = render_texture_create(r, width, height, format, TEXTURE_2D, 0);
	render_texture_update(r, id, width, height, data, 0, 0);
	return id;
}

void RenderContext::ReleaseTexture(RID id)
{
	render* r = sl_shader_get_render();
	render_release(r, TEXTURE, id);
}

}