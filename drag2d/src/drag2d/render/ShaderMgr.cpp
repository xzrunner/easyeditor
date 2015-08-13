#include "ShaderMgr.h"
#include "ShapeShader.h"
#include "SpriteShader.h"
#include "BlendShader.h"
#include "FontShader.h"
#include "ScreenShader.h"

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
		RenderContext::SetShader2DMgr(m_instance);
	}
	return m_instance;
}

ShaderMgr::ShaderMgr()
	: m_version(0)
{
	m_shape_shader = new ShapeShader;
	m_shaders.push_back(m_shape_shader);
	m_all_shape_shader.push_back(m_shape_shader);

 	m_sprite_shader = new SpriteShader;
 	m_shaders.push_back(m_sprite_shader);
	m_all_sprite_shader.push_back(m_sprite_shader);

	m_font_shader = new FontShader;
	m_shaders.push_back(m_font_shader);

	m_screen_shader = new ScreenShader;
	m_shaders.push_back(m_screen_shader);
}

ShaderMgr::~ShaderMgr()
{
	delete m_shape_shader;
	delete m_sprite_shader;
	delete m_font_shader;
}

void ShaderMgr::SetSpriteColor(const Colorf& multi, const Colorf& add)
{
	SpriteShader* shader = static_cast<SpriteShader*>(m_sprite_shader);
	shader->SetColor(multi, add);
}

void ShaderMgr::SetSpriteColorTrans(const Colorf& r_trans, const Colorf& g_trans, const Colorf& b_trans)
{
	SpriteShader* shader = static_cast<SpriteShader*>(m_sprite_shader);
	shader->SetColorTrans(r_trans, g_trans, b_trans);
}

void ShaderMgr::SetShapeColor(const Colorf& col)
{
	 m_shape_shader->SetColor(col);
}

void ShaderMgr::SetBlendMode(BlendMode mode)
{
	BlendShader* blend = dynamic_cast<BlendShader*>(m_sprite_shader);
	if (blend) {
		blend->SetBlendMode(BlendModes::Instance()->GetNameENFromID(mode));
	}
}

void ShaderMgr::sprite()
{
	RenderContext::Bind2d();
	Switch(m_sprite_shader);
}

void ShaderMgr::shape()
{
	RenderContext::Bind2d();
	Switch(m_shape_shader);
}

void ShaderMgr::Screen()
{
	RenderContext::Bind2d();
	Switch(m_screen_shader);
}

void ShaderMgr::null()
{
	RenderContext::Bind2d();
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
	for (int i = 0, n = m_all_sprite_shader.size(); i < n; ++i) {
	 	SpriteShader* shader = static_cast<SpriteShader*>(m_all_sprite_shader[i]);
	 	shader->SetTexID(tex);
	}
}

void ShaderMgr::SetFBO(int fbo)
{
	for (int i = 0, n = m_all_sprite_shader.size(); i < n; ++i) {
		SpriteShader* shader = static_cast<SpriteShader*>(m_all_sprite_shader[i]);
		shader->SetFboID(fbo);
	}
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

void ShaderMgr::DrawScreen(int texid)
{
	m_screen_shader->Draw(texid);
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
	for (int i = 0, n = m_all_shape_shader.size(); i < n; ++i) {
		m_all_shape_shader[i]->SetModelView(offset, scale);
	}
	for (int i = 0, n = m_all_sprite_shader.size(); i < n; ++i) {
		m_all_sprite_shader[i]->SetModelView(offset, scale);
	}
	
	m_offset = offset;
	m_scale = scale;
}

void ShaderMgr::GetModelView(Vector& offset, float& scale)
{
	offset = m_offset;
	scale = m_scale;
}

int ShaderMgr::AddShapeShader(d2d::ShapeShader* shader)
{
	m_all_shape_shader.push_back(shader);
	m_shaders.push_back(shader);
	return m_all_shape_shader.size() - 1;
}

void ShaderMgr::SetShapeShader(int idx)
{
	if (idx >= 0 && idx < m_all_shape_shader.size()) {
		if (m_shape_shader != m_all_shape_shader[idx]) {
			m_shape_shader = m_all_shape_shader[idx];
		}
	}
}

int ShaderMgr::AddSpriteShader(d2d::SpriteShader* shader)
{
	m_all_sprite_shader.push_back(shader);
	m_shaders.push_back(shader);
	return m_all_sprite_shader.size() - 1;
}

void ShaderMgr::SetSpriteShader(int idx)
{
	if (idx >= 0 && idx < m_all_sprite_shader.size()) {
		if (m_sprite_shader != m_all_sprite_shader[idx]) {
			m_sprite_shader = m_all_sprite_shader[idx];
		}
	}
}

void ShaderMgr::SetSpriteShader(d2d::SpriteShader* shader, bool delete_old)
{
	if (m_sprite_shader == shader) {
		return;
	}

	std::vector<IShader*>::iterator itr;
	for (itr = m_shaders.begin(); itr != m_shaders.end(); ++itr) {
		if (*itr == m_sprite_shader) {
			m_shaders.erase(itr);
			break;
		}
	}	

	if (delete_old) {
		m_sprite_shader->Unload();
		delete m_sprite_shader;
	}
	m_sprite_shader = shader;

	m_shaders.push_back(m_sprite_shader);
}

}