#ifndef _GLUE_SHADER_MGR_H_
#define _GLUE_SHADER_MGR_H_

#include "utility/macro.h"

#include <vector>

namespace glue
{

class IShader;

class ShaderMgr
{
public:
	enum Type
	{
		SHAPE = 0,
		SPRITE,
		BLEND,
		FILTER,
		SPRITE3,
		LIGHTING,

		COUNT,
	};

public:
	void Init();

	void OnSize(int width, int height);

	void SetShader(Type t);
	Type GetShader() const;
	IShader* GetShader(Type t) const;

	void Flush();

private:
	std::vector<IShader*> m_shaders;

	Type m_curr;
	
	SINGLETON_DECLARATION(ShaderMgr)

}; // ShaderMgr

}

#endif // _GLUE_SHADER_MGR_H_