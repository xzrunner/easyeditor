#include "ShaderImpl.h"

#include <stdio.h>

namespace d2d
{

ShaderImpl::ShaderImpl()
{
	m_curr_shader = NULL;
	m_version = 0;
	m_shape_shader = m_sprite_shader = m_font_shader = NULL;
	m_light_shader = NULL;
}

}