#include "ShaderMgr.h"
#include "ShapeShader.h"
#include "SpriteShader.h"
#include "BlendShader.h"
#include "ScreenShader.h"
#include "ModelShader.h"
#include "FilterShader.h"
#include "ShaderContext.h"

#include <sl_shader.h>

#include <stdio.h>

namespace ee
{

static const int MAX_TEXTURE = 4096;

ShaderMgr* ShaderMgr::m_instance = NULL;

ShaderMgr* ShaderMgr::Instance()
{
	if (!m_instance)
	{
		sl_shader_mgr_create(MAX_TEXTURE);

		m_instance = new ShaderMgr();
		ShaderContext::SetShader2DMgr(m_instance);
	}
	return m_instance;
}

ShaderMgr::ShaderMgr()
	: m_version(0)
{
	m_shaders.push_back(m_shape_shader = new ShapeShader);
//	m_all_shape_shader.push_back(m_shape_shader);

 	m_shaders.push_back(m_sprite_shader = new SpriteShader);
//	m_all_sprite_shader.push_back(m_sprite_shader);

	m_shaders.push_back(m_screen_shader = new ScreenShader);

  	m_shaders.push_back(m_blend_shader = new BlendShader);

	m_shaders.push_back(m_model_shader = new ModelShader);

	m_shaders.push_back(m_filter_shader = new FilterShader);
}

ShaderMgr::~ShaderMgr()
{
	for (int i = 0, n = m_shaders.size(); i < n; ++i) {
		delete m_shaders[i];
	}
}

void ShaderMgr::NullProg()
{
	Null();
}

void ShaderMgr::SetSpriteColor(const ColorTrans& color)
{
	m_sprite_shader->SetColor(color);
}

void ShaderMgr::SetShapeColor(const Colorf& col)
{
	 m_shape_shader->SetColor(col);
}

void ShaderMgr::SetBlendColor(const ColorTrans& color)
{
	m_blend_shader->SetColor(color);
}

void ShaderMgr::SetBlendMode(BlendMode mode)
{
	m_blend_shader->SetMode(BlendModes::Instance()->GetNameENFromID(mode));
}

void ShaderMgr::SetFilterMode(FilterMode mode)
{
	m_filter_shader->SetMode(FilterModes::Instance()->GetNameENFromID(mode));
}

void ShaderMgr::OnSize(int width, int height)
{
	m_blend_shader->OnSize(width, height);
}

void ShaderMgr::Sprite()
{
	ShaderContext::Bind2d();
	Switch(m_sprite_shader);
}

void ShaderMgr::Shape()
{
	ShaderContext::Bind2d();
	Switch(m_shape_shader);
}

void ShaderMgr::Screen()
{
	ShaderContext::Bind2d();
	Switch(m_screen_shader);
}

void ShaderMgr::Blend()
{
	ShaderContext::Bind2d();
	Switch(m_blend_shader);
}

void ShaderMgr::Model()
{
	ShaderContext::Bind2d();
	Switch(m_model_shader);	
}

void ShaderMgr::Filter()
{
	ShaderContext::Bind2d();
	Switch(m_filter_shader);	
}

void ShaderMgr::Null()
{
	ShaderContext::Bind2d();
	Switch(NULL);
}

int ShaderMgr::GetTexID() const 
{ 
	return sl_shader_get_texture();
}

int ShaderMgr::GetFboID() const 
{ 
	return sl_shader_get_target();
}

void ShaderMgr::SetTexture(int tex)
{
	sl_shader_set_texture(tex, 0);
}

void ShaderMgr::SetFBO(int fbo)
{
	sl_shader_set_target(fbo);
}

// void ShaderMgr::Draw(const float vb[16], int texid)
// {
// 	SpriteShader* shader = static_cast<SpriteShader*>(m_sprite_shader);
// 	shader->Draw(vb, texid);
// }

void ShaderMgr::Draw(const Vector vertices[4], const Vector texcoords[4], int texid)
{
	SpriteShader* shader = static_cast<SpriteShader*>(m_sprite_shader);
	shader->Draw(vertices, texcoords, texid);
}

void ShaderMgr::DrawScreen(int texid)
{
	m_screen_shader->Draw(texid);
}

void ShaderMgr::DrawBlend(const Vector vertices[4], const Vector texcoords[4], 
						  const Vector texcoords_base[4], int texid, int texid_base)
{
	m_blend_shader->Draw(vertices, texcoords, texcoords_base, texid, texid_base);
}

void ShaderMgr::DrawModel(const std::vector<vec3>& positions, const std::vector<Vector>& texcoords, int texid)
{
	m_model_shader->Draw(positions, texcoords, texid);
}

void ShaderMgr::DrawFilter(const Vector vertices[4], const Vector texcoords[4], int texid)
{
	m_filter_shader->Draw(vertices, texcoords, texid);
}

int ShaderMgr::GetVersion() const 
{
	return m_version; 
}

void ShaderMgr::SetBufferData(bool open) 
{
// 	SpriteShader* shader = static_cast<SpriteShader*>(m_sprite_shader);
// 	shader->SetBufferData(open);
}

bool ShaderMgr::IsOpenBufferData() const 
{ 
// 	SpriteShader* shader = static_cast<SpriteShader*>(m_sprite_shader);
// 	return shader->IsOpenBufferData();

	return false;
}

void ShaderMgr::SetModelView(const Vector& offset, float scale)
{
// 	for (int i = 0, n = m_all_shape_shader.size(); i < n; ++i) {
// 		m_all_shape_shader[i]->SetModelView(offset, scale);
// 	}
// 	for (int i = 0, n = m_all_sprite_shader.size(); i < n; ++i) {
// 		m_all_sprite_shader[i]->SetModelView(offset, scale);
// 	}

	m_shape_shader->SetModelView(offset, scale);
	m_sprite_shader->SetModelView(offset, scale);
	m_blend_shader->SetModelView(offset, scale);
	m_model_shader->SetModelView(offset, scale);
	m_filter_shader->SetModelView(offset, scale);
}

// int ShaderMgr::AddShapeShader(ShapeShader* shader)
// {
// 	m_all_shape_shader.push_back(shader);
// 	m_shaders.push_back(shader);
// 	return m_all_shape_shader.size() - 1;
// }
// 
// void ShaderMgr::SetShapeShader(int idx)
// {
// 	if (idx >= 0 && idx < static_cast<int>(m_all_shape_shader.size())) {
// 		if (m_shape_shader != m_all_shape_shader[idx]) {
// 			m_shape_shader = m_all_shape_shader[idx];
// 		}
// 	}
// }

// int ShaderMgr::AddSpriteShader(SpriteShader* shader)
// {
// 	m_all_sprite_shader.push_back(shader);
// 	m_shaders.push_back(shader);
// 	return m_all_sprite_shader.size() - 1;
// }

// void ShaderMgr::SetSpriteShader(int idx)
// {
// 	if (idx >= 0 && idx < static_cast<int>(m_all_sprite_shader.size())) {
// 		if (m_sprite_shader != m_all_sprite_shader[idx]) {
// 			m_sprite_shader = m_all_sprite_shader[idx];
// 		}
// 	}
// }

// void ShaderMgr::SetSpriteShader(SpriteShader* shader, bool delete_old)
// {
// 	if (m_sprite_shader == shader) {
// 		return;
// 	}
// 
// 	std::vector<IShader*>::iterator itr;
// 	for (itr = m_shaders.begin(); itr != m_shaders.end(); ++itr) {
// 		if (*itr == m_sprite_shader) {
// 			m_shaders.erase(itr);
// 			break;
// 		}
// 	}	
// 
// 	if (delete_old) {
// 		m_sprite_shader->Unload();
// 		delete m_sprite_shader;
// 	}
// 	m_sprite_shader = shader;
// 
// 	m_shaders.push_back(m_sprite_shader);
// }

bool ShaderMgr::IsCurrentBlendShader() const
{
	return m_curr_shader == m_blend_shader;
}

bool ShaderMgr::IsCurrentFilterShader() const
{
	return m_curr_shader == m_filter_shader;
}

}