#include "Texture.h"

namespace s2
{

Texture::Texture(uint16_t w, uint16_t h, uint32_t id) 
	: m_ori_w(0)
	, m_ori_h(0)
{
	Init(w, h, id);
}

void Texture::Init(uint16_t w, uint16_t h, uint32_t id) 
{
	m_width = w;
	m_height = h;
	m_tex_id = id;
}

void Texture::InitOri(uint16_t ori_w, uint16_t ori_h)
{
	m_ori_w = ori_w;
	m_ori_h = ori_h;
}

}