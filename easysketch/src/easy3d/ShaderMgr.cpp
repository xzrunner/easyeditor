#include "ShaderMgr.h"

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
	m_light_shader = new d2d::LightingShader;
	m_shaders.push_back(m_light_shader);
}

ShaderMgr::~ShaderMgr()
{
}

void ShaderMgr::Lighting()
{
	Switch(m_light_shader);	
}

void ShaderMgr::Draw(const z3d::IModel* model, const vec3& pos)
{
	m_light_shader->Draw(model, pos);
}

void ShaderMgr::SetModelView(const Quaternion& ori)
{
	m_light_shader->SetModelView(ori);
}

}