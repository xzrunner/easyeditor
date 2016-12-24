#include "RenderContext.h"

#include <ejoy2d/render.h>

#include <stdlib.h>

namespace ur
{

RenderContext::RenderContext()
{
	render_init_args RA;
	// todo: config these args
	RA.max_buffer  = 128;
	RA.max_layout  = MAX_LAYOUT;
	RA.max_target  = 128;
	RA.max_texture = MAX_TEXTURE;
	RA.max_shader  = MAX_SHADER;

	int smz = render_size(&RA);
	m_render = (render*)malloc(smz);
	m_render = render_init(&RA, m_render, smz);

	render_set_blendfunc(m_render, BLEND_ONE, BLEND_ONE_MINUS_SRC_ALPHA);
	render_set_blendeq(m_render, BLEND_FUNC_ADD);
}

RenderContext::~RenderContext()
{
	render_exit(m_render);
	free(m_render);
}

}