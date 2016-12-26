#ifndef _SHADERLAB_SHADER_MGR_H_
#define _SHADERLAB_SHADER_MGR_H_

#include <CU_Singleton.h>

#include "ShaderType.h"

#include <vector>

#include <stddef.h>

namespace ur { class RenderContext; }

namespace sl
{

class Shader;
class RenderShader;

class ShaderMgr
{
public:
	void SetContext(ur::RenderContext* context) { m_context = context; }
	ur::RenderContext* GetContext() { return m_context; }

	void CreateShader(ShaderType type, Shader* shader);
	void ReleaseShader(ShaderType type);

	void SetShader(ShaderType type);
	Shader* GetShader() const {
		return m_curr_shader == -1 ? NULL : m_shaders[m_curr_shader];
	}
	Shader* GetShader(ShaderType type) const {
		return m_shaders[type];
	}
	ShaderType GetShaderType() const {
		return m_curr_shader == -1 ? MAX_SHADER : (ShaderType)m_curr_shader;
	}

	RenderShader* CreateRenderShader();
	void BindRenderShader(RenderShader* shader, int type = -1);

	void Flush();
	
private:
	ur::RenderContext* m_context;

	Shader* m_shaders[MAX_SHADER];
	int m_curr_shader;

	std::vector<RenderShader*> m_render_shaders;
	RenderShader* m_curr_render_shader;

	SINGLETON_DECLARATION(ShaderMgr);

}; // ShaderMgr

}

#endif // _SHADERLAB_SHADER_MGR_H_