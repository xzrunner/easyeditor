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

void ShaderLab::Init()
{
	sl_shader_mgr_create();
}

int ShaderLab::CreateTexture(const uint8_t* data, int width, int height, int format)
{
	TEXTURE_FORMAT ej_fmt;
	switch (format)
	{
	case GL_RGBA:
		ej_fmt = TEXTURE_RGBA8;
		break;
	case GL_RGB:
		ej_fmt = TEXTURE_RGB;
		break;
	case GL_LUMINANCE:
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

}