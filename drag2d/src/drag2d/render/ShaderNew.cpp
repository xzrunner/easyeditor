#include "ShaderNew.h"
#include "ShaderImpl.h"
#include "ShapeShader.h"
#include "SpriteShader.h"
#include "FontShader.h"

#include <stdio.h>

namespace d2d
{

ShaderNew* ShaderNew::m_instance = NULL;

ShaderNew* ShaderNew::Instance()
{
	if (!m_instance)
	{
		m_instance = new ShaderNew();
	}
	return m_instance;
}

ShaderNew::ShaderNew()
{
	m_impl = new ShaderImpl;

	m_impl->m_shape_shader = new ShapeShader;
	m_impl->m_sprite_shader = new SpriteShader;
	m_impl->m_font_shader = new FontShader;
}

ShaderNew::~ShaderNew()
{
	delete m_impl;
}

void ShaderNew::SetSpriteColor(const Colorf& multi, const Colorf& add)
{
	SpriteShader* shader = static_cast<SpriteShader*>(m_impl->m_sprite_shader);
	shader->SetColor(multi, add);
}

void ShaderNew::SetShapeColor(const Colorf& col)
{
	if (m_impl->m_shape_shader == m_impl->m_curr_shader) {
	 	ShapeShader* shader = static_cast<ShapeShader*>(m_impl->m_shape_shader);
	 	shader->SetColor(col);
	}
}

void ShaderNew::sprite()
{
	if (m_impl->m_curr_shader != m_impl->m_sprite_shader)
	{
		if (m_impl->m_curr_shader) {
			m_impl->m_curr_shader->Commit();
			m_impl->m_curr_shader->Unbind();
		}
		m_impl->m_curr_shader = m_impl->m_sprite_shader;
		m_impl->m_curr_shader->Bind();
	}
}

void ShaderNew::shape()
{
	if (m_impl->m_curr_shader != m_impl->m_shape_shader)
	{
		if (m_impl->m_curr_shader) {
			m_impl->m_curr_shader->Commit();
			m_impl->m_curr_shader->Unbind();
		}
		m_impl->m_curr_shader = m_impl->m_shape_shader;
		m_impl->m_curr_shader->Bind();
	}
}

void ShaderNew::null()
{
	int zz = 0;

// 	if (m_impl->m_sprite_count != 0) {
// 		wxLogDebug(_T("Shader Commit change shader to null"));
// 	}
// 
// 	Commit();
// 
// 	glEnable(GL_BLEND);
// 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
// 
// 	glUseProgram(0);
// 	m_impl->m_prog_curr = 0;
}

void ShaderNew::release()
{
	m_impl->m_shape_shader->Unload();
	m_impl->m_sprite_shader->Unload();
	m_impl->m_font_shader->Unload();
}

void ShaderNew::reload()
{
	m_impl->m_shape_shader->Load();
	m_impl->m_sprite_shader->Load();
	m_impl->m_font_shader->Load();
}

int ShaderNew::GetTexID() const 
{ 
	SpriteShader* shader = static_cast<SpriteShader*>(m_impl->m_sprite_shader);
	return shader->GetTexID();
}

int ShaderNew::GetFboID() const 
{ 
	SpriteShader* shader = static_cast<SpriteShader*>(m_impl->m_sprite_shader);
	return shader->GetFboID();
}

void ShaderNew::SetTexture(int tex)
{
	SpriteShader* shader = static_cast<SpriteShader*>(m_impl->m_sprite_shader);
	shader->SetTexID(tex);
}

void ShaderNew::SetFBO(int fbo)
{
	SpriteShader* shader = static_cast<SpriteShader*>(m_impl->m_sprite_shader);
	shader->SetFboID(fbo);
}

void ShaderNew::Draw(const float vb[16], int texid)
{
	SpriteShader* shader = static_cast<SpriteShader*>(m_impl->m_sprite_shader);
	shader->Draw(vb, texid);
}

void ShaderNew::Draw(const Vector vertices[4], const Vector texcoords[4], int texid)
{
	SpriteShader* shader = static_cast<SpriteShader*>(m_impl->m_sprite_shader);
	shader->Draw(vertices, texcoords, texid);
}

int ShaderNew::GetVersion() const 
{
	return m_impl->m_version; 
}

void ShaderNew::SetBufferData(bool open) 
{
	SpriteShader* shader = static_cast<SpriteShader*>(m_impl->m_sprite_shader);
	shader->SetBufferData(open);
}

bool ShaderNew::IsOpenBufferData() const 
{ 
	SpriteShader* shader = static_cast<SpriteShader*>(m_impl->m_sprite_shader);
	return shader->IsOpenBufferData();
}

void ShaderNew::SetModelView(const Vector& offset, float scale)
{
	m_impl->m_shape_shader->SetModelView(offset, scale);
	m_impl->m_sprite_shader->SetModelView(offset, scale);
	
	m_impl->m_offset = offset;
	m_impl->m_scale = scale;
}

void ShaderNew::GetModelView(Vector& offset, float& scale)
{
	offset = m_impl->m_offset;
	scale = m_impl->m_scale;
}

void ShaderNew::SetProjection(int width, int height)
{
	m_impl->m_shape_shader->SetProjection(width, height);
	m_impl->m_sprite_shader->SetProjection(width, height);
}

void ShaderNew::Commit()
{
	if (m_impl->m_curr_shader) {
		m_impl->m_curr_shader->Commit();
	}
}

void ShaderNew::Flush()
{
	++m_impl->m_version;

// 	if (m_sprite_count != 0) {
// 		wxLogDebug(_T("Shader Commit Flush"));
// 	}

	Commit();

//	DynamicTexture::Instance()->DebugDraw();
}

}