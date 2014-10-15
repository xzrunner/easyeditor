#include "ShaderImpl.h"

#include <stdio.h>

namespace d2d
{

ShaderImpl::ShaderImpl()
{
	m_version = 0;
	m_shape_shader = m_sprite_shader = m_font_shader = NULL;
}

}