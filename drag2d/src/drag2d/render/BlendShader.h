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
	virtual void BindAttrib(GLuint prog);

	virtual void LoadShader();

private:
	BlendMode m_mode;

	typedef int GLint;
	GLint m_mode_id;

}; // BlendShader

}

#endif // _DRAG2D_BLEND_SHADER_H_