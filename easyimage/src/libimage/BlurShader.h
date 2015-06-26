#ifndef _EASYIMAGE_BLUR_SHADER_H_
#define _EASYIMAGE_BLUR_SHADER_H_

#include <drag2d.h>

namespace eimage
{

class BlurShader : public d2d::SpriteShader
{
public:
	BlurShader();

	void SetRadius(float r);

protected:
	virtual void LoadShader();

private:
	GLuint m_radius;

}; // BlurShader

}

#endif // _EASYIMAGE_BLUR_SHADER_H_