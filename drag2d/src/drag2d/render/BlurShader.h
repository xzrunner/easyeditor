#ifndef _DRAG2D_BLUR_SHADER_H_
#define _DRAG2D_BLUR_SHADER_H_

#include "SpriteShader.h"

namespace d2d
{

class BlurShader : public SpriteShader
{
public:
	BlurShader();

	virtual void Bind();

	void SetRadius(float r);

protected:
	virtual void LoadShader();

private:
	GLuint m_radius;

}; // BlurShader

}

#endif // _DRAG2D_BLUR_SHADER_H_