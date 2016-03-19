#include "ShaderMgr.h"
#include "SpriteShader.h"
#include "ShapeShader.h"
#include "BlendShader.h"
#include "Sprite3Shader.h"
//#include "ScreenCache.h"

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
	: m_curr_shader(-1)
{
}

ShaderMgr::~ShaderMgr() 
{
	if (m_curr_shader != -1) {
		m_shaders[m_curr_shader]->Commit();
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
	m_shaders.push_back(new SpriteShader);
	m_shaders.push_back(new ShapeShader);
	m_shaders.push_back(new BlendShader);
	m_shaders.push_back(new Sprite3Shader);
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

void ShaderMgr::SpriteDraw(const vec2 vertices[4], const vec2 texcoords[4], int texid)
{
	ChangeShader(SPRITE_IDX);
	static_cast<SpriteShader*>(m_shaders[SPRITE_IDX])->Draw(vertices, texcoords, texid);
}

void ShaderMgr::ShapeDraw()
{
	ChangeShader(SHAPE_IDX);
}

void ShaderMgr::BlendDraw(const vec2 vertices[4], const vec2 texcoords[4], 
						  const vec2 texcoords_base[4], int tex_blend, int tex_base)
{
	ChangeShader(BLEND_IDX);
}

void ShaderMgr::ModelDraw(const std::vector<vec3>& vertices, const std::vector<vec2>& texcoords, int texid)
{
	ChangeShader(MODEL_IDX);
	static_cast<Sprite3Shader*>(m_shaders[MODEL_IDX])->Draw(vertices, texcoords, texid);
}

void ShaderMgr::SetSpriteColor(uint32_t color, uint32_t additive)
{
	static_cast<SpriteShader*>(m_shaders[SPRITE_IDX])->SetColor(color, additive);
}

void ShaderMgr::SetSpriteMapColor(uint32_t rmap, uint32_t gmap, uint32_t bmap)
{
	static_cast<SpriteShader*>(m_shaders[SPRITE_IDX])->SetMapColor(rmap, gmap, bmap);
}

void ShaderMgr::SetBlendColor(uint32_t color, uint32_t additive)
{
	static_cast<BlendShader*>(m_shaders[BLEND_IDX])->SetColor(color, additive);
}

void ShaderMgr::SetBlendMode(BlendMode mode)
{
	static_cast<BlendShader*>(m_shaders[BLEND_IDX])->SetMode(mode);
}

bool ShaderMgr::IsBlendShader() const
{
	return m_curr_shader == BLEND_IDX;
}

void ShaderMgr::Flush()
{
	if (m_curr_shader != -1) {
		m_shaders[m_curr_shader]->Commit();
	}

	sl_shader_flush();
}

void ShaderMgr::ChangeShader(int idx)
{
	if (m_curr_shader == idx) {
		return;
	}
	if (m_curr_shader != -1 && m_curr_shader != idx) {
		IShader* old = m_shaders[m_curr_shader];
		old->Commit();
		old->Unbind();
	}
	m_curr_shader = idx;
	m_shaders[m_curr_shader]->Bind();
}

}