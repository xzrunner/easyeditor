#include "ShaderMgr.h"
#include "ShapeShader.h"
#include "SpriteShader.h"
#include "BlendShader.h"
#include "FilterShader.h"
#include "Sprite3Shader.h"
#include "LightingShader.h"

#include <sl_shader.h>

#include <stddef.h>

namespace glue
{

static const int SPRITE_IDX		= 0;
static const int SHAPE_IDX		= 1;
static const int BLEND_IDX		= 2;
static const int MODEL_IDX		= 3;

SINGLETON_DEFINITION(ShaderMgr)

ShaderMgr::ShaderMgr() 
	: m_curr(ShaderMgr::COUNT)
{
}

ShaderMgr::~ShaderMgr() 
{
	if (m_curr != ShaderMgr::COUNT) {
		m_shaders[m_curr]->Commit();
	}
	for (int i = 0, n = m_shaders.size(); i < n; ++i) {
		IShader* shader = m_shaders[i];
		shader->Unload();
		delete shader;
	}
	m_shaders.clear();
}

void ShaderMgr::Init()
{
	m_shaders.push_back(new ShapeShader);
	m_shaders.push_back(new SpriteShader);
	m_shaders.push_back(new BlendShader);
	m_shaders.push_back(new FilterShader);
	m_shaders.push_back(new Sprite3Shader);
	m_shaders.push_back(new LightingShader);
	for (int i = 0, n = m_shaders.size(); i < n; ++i) {
		m_shaders[i]->Load();
	}
}

void ShaderMgr::OnSize(int width, int height)
{
//	ScreenCache::Instance()->SetSize(width, height);

	for (int i = 0, n = m_shaders.size(); i < n; ++i) {
		m_shaders[i]->OnSize(width, height);
	}
}

void ShaderMgr::SetShader(ShaderMgr::Type t)
{
	if (m_curr == t) {
		return;
	}
	if (m_curr != ShaderMgr::COUNT) {
		IShader* old = m_shaders[m_curr];
		old->Commit();
		old->Unbind();
	}
	m_curr = t;
	m_shaders[m_curr]->Bind();
}

ShaderMgr::Type ShaderMgr::GetShader() const
{
	return m_curr;
}

IShader* ShaderMgr::GetShader(ShaderMgr::Type t) const
{
	if (t == ShaderMgr::COUNT) {
		return NULL;
	} else {
		return m_shaders[t];
	}
}

void ShaderMgr::Flush()
{
	if (m_curr != ShaderMgr::COUNT) {
		m_shaders[m_curr]->Commit();
	}

	sl_shader_flush();
}

}