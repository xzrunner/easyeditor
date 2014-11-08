#include "ShaderMgr.h"
#include "LightingShader.h"
#include "ShapeShader.h"
#include "SpriteShader.h"

namespace e3d
{

ShaderMgr* ShaderMgr::m_instance = NULL;

ShaderMgr* ShaderMgr::Instance()
{
	if (!m_instance)
	{
		m_instance = new ShaderMgr();
		d2d::RenderContext::SHADER_MGR = m_instance;
	}
	return m_instance;
}

ShaderMgr::ShaderMgr()
{
	m_light_shader = new LightingShader;
	m_shaders.push_back(m_light_shader);

	m_shape_shader = new ShapeShader;
	m_shaders.push_back(m_shape_shader);

	m_sprite_shader = new SpriteShader;
	m_shaders.push_back(m_sprite_shader);
}

ShaderMgr::~ShaderMgr()
{
}

void ShaderMgr::Lighting()
{
	Switch(m_light_shader);
}

void ShaderMgr::Shape()
{
	Switch(m_shape_shader);
}

void ShaderMgr::Sprite()
{
	Switch(m_sprite_shader);	
}

void ShaderMgr::DrawModel(const IModel* model, const mat4& m)
{
	m_light_shader->Draw(model, m);
}

void ShaderMgr::DrawTri(const vec3 vertices[3], const vec2 texcoords[3], int texid)
{
	m_sprite_shader->DrawTri(vertices, texcoords, texid);
}

void ShaderMgr::SetModelView(const mat4& mat)
{
	m_light_shader->SetModelView(mat);
	m_shape_shader->SetModelView(mat);
	m_sprite_shader->SetModelView(mat);
}

void ShaderMgr::SetShapeColor(const d2d::Colorf& col)
{
	m_shape_shader->SetColor(col);
}

}