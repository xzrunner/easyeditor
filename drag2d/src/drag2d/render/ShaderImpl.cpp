#include "ShaderImpl.h"

namespace d2d
{

//#define BUFFER_OFFSET(i) ((char *)NULL + (i))
//
//static const int ATTRIB_VERTEX    = 0;
//static const int ATTRIB_TEXTCOORD = 1;
//static const int ATTRIB_COLOR     = 2;
//static const int ATTRIB_ADDITIVE  = 3;

int ShaderImpl::MAX_COMMBINE = 20000;
//int ShaderImpl::MAX_COMMBINE = 4096;

ShaderImpl::ShaderImpl()
{
	m_prog_curr = 0;

	VertexBuffer = IndexBuffer = 0;
	m_sprite_count = 0;
	m_vb = NULL;
	m_tex = 0;
	m_fbo = 0;

	m_color = 0xffffffff;
	m_additive = 0;

	m_version = 0;

	m_open_buffer_data = true;
}

}