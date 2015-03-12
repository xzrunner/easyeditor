#ifndef _EASY2D_SHADER_MGR_BASE_H_
#define _EASY2D_SHADER_MGR_BASE_H_

#include <vector>

namespace e2d
{

class IShader;

class ShaderMgrBase
{
public:
	ShaderMgrBase();
	~ShaderMgrBase();

	void Release();
	void reload();

	// todo
	void Commit();

	void SetProjection(int width, int height);

	void Flush();

protected:
	void Switch(IShader* shader);

protected:
	IShader* m_curr_shader;

	std::vector<IShader*> m_shaders;

}; // ShaderMgrBase

}

#endif // _EASY2D_SHADER_MGR_BASE_H_