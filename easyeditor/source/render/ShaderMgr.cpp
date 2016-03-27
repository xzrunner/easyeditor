#include "ShaderMgr.h"
#include "ShapeShader.h"
#include "SpriteShader.h"
#include "BlendShader.h"
#include "Sprite3Shader.h"
#include "FilterShader.h"
#include "ShaderContext.h"

#include <sl_shader.h>

#include <stdio.h>

namespace ee
{

static const int MAX_TEXTURE = 4096;

ShaderMgr* ShaderMgr::m_instance = NULL;

static void
_commit() {
	ShaderMgr::Instance()->Commit();
}

ShaderMgr* ShaderMgr::Instance()
{
	if (!m_instance)
	{
		sl_shader_mgr_create(MAX_TEXTURE, _commit);

		m_instance = new ShaderMgr();
		ShaderContext::SetShader2DMgr(m_instance);
	}
	return m_instance;
}

ShaderMgr::ShaderMgr()
	: m_curr(ShaderMgr::COUNT)
{
	m_shaders.push_back(new ShapeShader);
	m_shaders.push_back(new SpriteShader);
	m_shaders.push_back(new BlendShader);
	m_shaders.push_back(new FilterShader);
	m_shaders.push_back(new Sprite3Shader);
}

ShaderMgr::~ShaderMgr()
{
	for (int i = 0, n = m_shaders.size(); i < n; ++i) {
		delete m_shaders[i];
	}
}

void ShaderMgr::NullProg()
{
	SetShader(COUNT);
}

void ShaderMgr::SetShader(ShaderMgr::Type t)
{
	ShaderContext::Bind2d();
	if (m_curr != t) {
		Switch(m_shaders[t]);
		m_curr = t;
	}
}

ShaderMgr::Type ShaderMgr::GetShader() const
{
	return m_curr;
}

IShader* ShaderMgr::GetShader(Type t) const
{
	if (t == ShaderMgr::COUNT) {
		return NULL;
	} else {
		return m_shaders[t];
	}
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
	static_cast<ShapeShader*>(m_shaders[SHAPE])->SetModelView(offset, scale);
	static_cast<SpriteShader*>(m_shaders[SPRITE])->SetModelView(offset, scale);
	static_cast<BlendShader*>(m_shaders[BLEND])->SetModelView(offset, scale);
	static_cast<FilterShader*>(m_shaders[FILTER])->SetModelView(offset, scale);
}

void ShaderMgr::SetModelView(const vec3& pos, float angle)
{
	static_cast<Sprite3Shader*>(m_shaders[SPRITE3])->SetModelView(pos, angle);	
}

}