#include "ShaderMgr.h"
#include "SpriteShader.h"
#include "ShapeShader.h"
#include "BlendShader.h"

#include <sl_shader.h>

#include <stddef.h>

namespace glue
{

SINGLETON_DEFINITION(ShaderMgr)

ShaderMgr::ShaderMgr() 
	: m_curr_shader(NULL)
	, m_sprite(NULL)
	, m_shape(NULL)
	, m_blend(NULL)
{
}

ShaderMgr::~ShaderMgr() 
{
	if (m_curr_shader) {
		m_curr_shader->Commit();
	}
	if (m_sprite) {
		m_sprite->Unload();
		delete m_sprite;
	}
	if (m_shape) {
		m_shape->Unload();
		delete m_shape;
	}
}

void ShaderMgr::Init()
{
	m_sprite = new SpriteShader;
	m_sprite->Load();

	m_shape = new ShapeShader;
	m_shape->Load();

	m_blend = new BlendShader;
	m_blend->Load();
}

void ShaderMgr::OnSize(int width, int height)
{
	m_sprite->OnSize(width, height);
	m_shape->OnSize(width, height);
	m_blend->OnSize(width, height);
}

void ShaderMgr::SpriteDraw(const vec2 vertices[4], const vec2 texcoords[4], int texid)
{
	ChangeShader(m_sprite);
	m_sprite->Draw(vertices, texcoords, texid);
}

void ShaderMgr::ShapeDraw()
{
	ChangeShader(m_shape);
	m_curr_shader = m_shape;
}

void ShaderMgr::BlendDraw(const vec2 vertices[4], const vec2 texcoords[4], 
						  const vec2 texcoords_base[4], int tex_blend, int tex_base)
{
	ChangeShader(m_blend);
	m_curr_shader = m_blend;
}

void ShaderMgr::SetSpriteColor(uint32_t color, uint32_t additive)
{
	m_sprite->SetColor(color, additive);
}

void ShaderMgr::SetSpriteMapColor(uint32_t rmap, uint32_t gmap, uint32_t bmap)
{
	m_sprite->SetMapColor(rmap, gmap, bmap);
}

void ShaderMgr::SetBlendColor(uint32_t color, uint32_t additive)
{
	m_blend->SetColor(color, additive);
}

void ShaderMgr::SetBlendMode(BlendMode mode)
{
	m_blend->SetMode(mode);
}

bool ShaderMgr::IsBlendShader() const
{
	return m_curr_shader == m_blend;
}

void ShaderMgr::Flush()
{
	if (m_curr_shader) {
		m_curr_shader->Commit();
	}

	sl_shader_flush();
}

void ShaderMgr::ChangeShader(IShader* shader)
{
	if (m_curr_shader == shader) {
		return;
	}
	if (m_curr_shader && m_curr_shader != shader) {
		m_curr_shader->Commit();
		m_curr_shader->Unbind();
	}
	m_curr_shader = shader;
	m_curr_shader->Bind();
}

}