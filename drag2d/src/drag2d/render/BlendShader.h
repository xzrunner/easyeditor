#ifndef _DRAG2D_BLEND_SHADER_H_
#define _DRAG2D_BLEND_SHADER_H_

#include "SpriteShader.h"
#include "BlendModes.h"

namespace d2d
{

class BlendShader : public SpriteShader
{
public:
	BlendShader();

	void SetBaseTexID(int tex);

	void SetBlendMode(const std::string& mode);
	
protected:
	virtual void GetUniformLocation(GLuint prog);

	virtual void LoadShader();

private:
	BlendMode m_mode;

	typedef int GLint;
	GLint m_mode_id;
	GLint m_sampler0, m_sampler1;

}; // BlendShader

}

#endif // _DRAG2D_BLEND_SHADER_H_