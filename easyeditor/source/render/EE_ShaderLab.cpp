#include "EE_ShaderLab.h"
#include "Exception.h"

#include <sl_shader.h>
#include <render/render.h>

#include <gl/glew.h>

#include <stddef.h>

namespace ee
{

SINGLETON_DEFINITION(ShaderLab)

ShaderLab::ShaderLab() 
{
}

ShaderLab::~ShaderLab() 
{
}

int ShaderLab::CreateTexture(const uint8_t* data, int width, int height, EE_TEXTURE_FORMAT format)
{
	TEXTURE_FORMAT ej_fmt;
	switch (format)
	{
	case EE_TEXTURE_RGBA8:
		ej_fmt = TEXTURE_RGBA8;
		break;
	case EE_TEXTURE_RGB:
		ej_fmt = TEXTURE_RGB;
		break;
	case EE_TEXTURE_A8:
		ej_fmt = TEXTURE_A8;
		break;
 	default:
 		throw ee::Exception("Unknown format %d", format);
	}

	render* r = sl_shader_get_render();
	RID id = render_texture_create(r, width, height, ej_fmt, TEXTURE_2D, 0);
	render_texture_update(r, id, width, height, data, 0, 0);
	return id;
}

void ShaderLab::ReleaseTexture(int id)
{
	render* r = sl_shader_get_render();
	render_release(r, TEXTURE, id);
}

void ShaderLab::UpdateTexture(const uint8_t* data, int x, int y, int w, int h, int id)
{
	render* r = sl_shader_get_render();
	render_texture_subupdate(r, id, data, x, y, w, h);
}

int ShaderLab::GetTexGLID(int tex)
{
	render* r = sl_shader_get_render();
	return render_query_tex_glid(r, tex);	
}

void ShaderLab::Flush()
{
	sl_shader_flush();
}

}