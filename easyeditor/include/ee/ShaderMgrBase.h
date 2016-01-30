#ifndef _EASYEDITOR_SHADER_MGR_BASE_H_
#define _EASYEDITOR_SHADER_MGR_BASE_H_

#include <vector>

namespace ee
{

class IShader;

class ShaderMgrBase
{
public:
	ShaderMgrBase();
	~ShaderMgrBase();

	virtual void NullProg() = 0;

	void Bind();
	void UnBind();

	void Release();
	void reload();

	// todo
	void Commit();

	void SetProjection(int width, int height);

	void Flush();

	void Reset();

protected:
	void Switch(IShader* shader);

protected:
	IShader* m_curr_shader;

	std::vector<IShader*> m_shaders;

}; // ShaderMgrBase

}

#endif // _EASYEDITOR_SHADER_MGR_BASE_H_