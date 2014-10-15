#include "ShaderMgr.h"
#include "ShapeShader.h"
#include "SpriteShader.h"
#include "FontShader.h"

#include "RenderContext.h"

#include <stdio.h>

namespace d2d
{

ShaderMgr* ShaderMgr::m_instance = NULL;

ShaderMgr* ShaderMgr::Instance()
{
	if (!m_instance)
	{
		m_instance = new ShaderMgr();
		RenderContext::SHADER_MGR = m_instance;
	}
	return m_instance;
}

ShaderMgr::ShaderMgr()
	: m_version(0)
{
	m_shape_shader = new ShapeShader;
	m_shaders.push_back(m_shape_shader);

	m_sprite_shader = new SpriteShader;
	m_shaders.push_back(m_sprite_shader);

	m_font_shader = new FontShader;
	m_shaders.push_back(m_font_shader);
}

ShaderMgr::~ShaderMgr()
{
}

void ShaderMgr::SetSpriteColor(const Colorf& multi, const Colorf& add)
{
	SpriteShader* shader = static_cast<SpriteShader*>(m_sprite_shader);
	shader->SetColor(multi, add);
}

void ShaderMgr::SetShapeColor(const Colorf& col)
{
//	if (m_shape_shader == m_curr_shader) {
	 	ShapeShader* shader = static_cast<ShapeShader*>(m_shape_shader);
	 	shader->SetColor(col);
//	}
}

void ShaderMgr::sprite()
{
	Switch(m_sprite_shader);
}

void ShaderMgr::shape()
{
	Switch(m_shape_shader);
}

void ShaderMgr::null()
{
	Switch(NULL);
}

int ShaderMgr::GetTexID() const 
{ 
	SpriteShader* shader = static_cast<SpriteShader*>(m_sprite_shader);
	return shader->GetTexID();
}

int ShaderMgr::GetFboID() const 
{ 
	SpriteShader* shader = static_cast<SpriteShader*>(m_sprite_shader);
	return shader->GetFboID();
}

void ShaderMgr::SetTexture(int tex)
{
	SpriteShader* shader = static_cast<SpriteShader*>(m_sprite_shader);
	shader->SetTexID(tex);
}

void ShaderMgr::SetFBO(int fbo)
{
	SpriteShader* shader = static_cast<SpriteShader*>(m_sprite_shader);
	shader->SetFboID(fbo);
}

void ShaderMgr::Draw(const float vb[16], int texid)
{
	SpriteShader* shader = static_cast<SpriteShader*>(m_sprite_shader);
	shader->Draw(vb, texid);
}

void ShaderMgr::Draw(const Vector vertices[4], const Vector texcoords[4], int texid)
{
	SpriteShader* shader = static_cast<SpriteShader*>(m_sprite_shader);
	shader->Draw(vertices, texcoords, texid);
}

int ShaderMgr::GetVersion() const 
{
	return m_version; 
}

void ShaderMgr::SetBufferData(bool open) 
{
	SpriteShader* shader = static_cast<SpriteShader*>(m_sprite_shader);
	shader->SetBufferData(open);
}

bool ShaderMgr::IsOpenBufferData() const 
{ 
	SpriteShader* shader = static_cast<SpriteShader*>(m_sprite_shader);
	return shader->IsOpenBufferData();
}

void ShaderMgr::SetModelView(const Vector& offset, float scale)
{
	m_shape_shader->SetModelView(offset, scale);
	m_sprite_shader->SetModelView(offset, scale);
	
	m_offset = offset;
	m_scale = scale;
}

void ShaderMgr::GetModelView(Vector& offset, float& scale)
{
	offset = m_offset;
	scale = m_scale;
}

}