#ifndef _EASYIMAGE_LANCZOS_RESAMPLING_SHADER_H_
#define _EASYIMAGE_LANCZOS_RESAMPLING_SHADER_H_

#include <drag2d.h>

namespace eimage
{

class LanczosResamplingShader : public d2d::SpriteShader
{
public:
	LanczosResamplingShader();

	void SetTexelSize(float width, float height);
	
protected:
	virtual void LoadShader();

private:
	GLuint m_texel_width, m_texel_height;

}; // LanczonsResamplingShader

}

#endif // _EASYIMAGE_LANCZOS_RESAMPLING_SHADER_H_
