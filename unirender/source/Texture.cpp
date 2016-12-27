#include "Texture.h"
#include "RenderContext.h"

#include <stdint.h>

namespace ur
{

Texture::Texture(RenderContext* rc, int width, int height)
	: m_rc(rc)
	, m_width(width)
	, m_height(height)
	, m_format(0)
	, m_id(0)
{
	Init();
}

Texture::~Texture()
{
	m_rc->ReleaseTexture(m_id);
}

void Texture::Bind() const
{
	m_rc->BindTexture(m_id, 0);
}

void Texture::Init()
{
	m_format = TEXTURE_RGBA8;

	uint32_t* pixels = new uint32_t[m_width * m_height];
	memset(pixels, 0, m_width * m_height * sizeof(uint32_t));

	m_id = m_rc->CreateTexture(pixels, m_width, m_height, m_format);

	delete[] pixels;
}

}