#ifndef _EASYEDITOR_BLUR_SHADER_H_
#define _EASYEDITOR_BLUR_SHADER_H_

#include "SpriteShader.h"

namespace ee
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

#endif // _EASYEDITOR_BLUR_SHADER_H_