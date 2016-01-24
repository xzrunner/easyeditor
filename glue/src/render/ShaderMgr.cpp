#include "ShaderMgr.h"
#include "SpriteShader.h"
#include "ShapeShader.h"

#include <sl_shader.h>

#include <stddef.h>

namespace glue
{

SINGLETON_DEFINITION(ShaderMgr)

ShaderMgr::ShaderMgr() 
	: m_curr_shader(NULL)
	, m_sprite(NULL)
	, m_shape(NULL)
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
}

void ShaderMgr::OnSize(int width, int height)
{
	m_sprite->OnSize(width, height);
	m_shape->OnSize(width, height);
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

void ShaderMgr::Flush()
{
	if (m_curr_shader) {
		m_curr_shader->Commit();
	}

	sl_shader_flush();
}

void ShaderMgr::ChangeShader(IShader* shader)
{
	if (m_curr_shader && m_curr_shader != shader) {
		m_curr_shader->Commit();
		m_curr_shader->Unbind();
	}
	m_curr_shader = shader;
	m_curr_shader->Bind();
}

}