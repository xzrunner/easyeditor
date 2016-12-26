#include "ShaderMgr.h"
#include "Shader.h"
#include "RenderShader.h"

#include <string.h>

namespace sl
{

SINGLETON_DEFINITION(ShaderMgr);

ShaderMgr::ShaderMgr()
	: m_curr_shader(-1)
{
	memset(m_shaders, 0, sizeof(m_shaders));

	m_render_shaders.reserve(MAX_SHADER);
	m_curr_render_shader = NULL;
}

ShaderMgr::~ShaderMgr()
{
	for (int i = 0, n = MAX_SHADER; i < n; ++i) {
		if (m_shaders[i]) {
			delete m_shaders[i];
		}
	}
	for (int i = 0, n = m_render_shaders.size(); i < n; ++i) {
		if (m_render_shaders[i]) {
			delete m_render_shaders[i];
		}
	}
}

void ShaderMgr::CreateShader(ShaderType type, Shader* shader)
{
	if (m_shaders[type]) {
		delete m_shaders[type];
	}
	m_shaders[type] = shader;
}

void ShaderMgr::ReleaseShader(ShaderType type)
{
	if (m_shaders[type]) {
		delete m_shaders[type];
		m_shaders[type] = NULL;
	}
}

void ShaderMgr::SetShader(ShaderType type)
{
	if (type == m_curr_shader) {
		return;
	}

	if (m_curr_shader != -1 && m_shaders[m_curr_shader]) {
		m_shaders[m_curr_shader]->Commit();
		m_shaders[m_curr_shader]->UnBind();
	}
	m_curr_shader = type;
	if (m_shaders[m_curr_shader]) {
		m_shaders[m_curr_shader]->Bind();
	}
}

RenderShader* ShaderMgr::CreateRenderShader()
{
	if (m_render_shaders.size() < MAX_SHADER) {
		RenderShader* shader = new RenderShader(m_context);
		m_render_shaders.push_back(shader);
		return shader;
	} else {
		return NULL;
	}
}

void ShaderMgr::BindRenderShader(RenderShader* shader, int type)
{
	if (m_curr_render_shader == shader) {
		return;
	}

	if (m_curr_render_shader && m_curr_render_shader->IsUniformChanged()) {
#ifdef SL_DC_STAT
		std::cout << "SL DC for change shader: " << type << '\n';
#endif // SL_DC_STAT
		m_curr_render_shader->Commit();
	}

	m_curr_render_shader = shader;
	m_curr_render_shader->Bind();
}

void ShaderMgr::Flush()
{
	Shader* shader = GetShader();
	if (shader) {
		shader->Commit();
	}
}

}