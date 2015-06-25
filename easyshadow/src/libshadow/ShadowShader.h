#ifndef _EASYSHADOW_SHADOW_SHADER_H_
#define _EASYSHADOW_SHADOW_SHADER_H_

#include <drag2d.h>

namespace eshadow
{

class ShadowShader : public d2d::ShapeShader
{
public:
	ShadowShader();

	void SetAlpha(float alpha);

protected:
	virtual void LoadShader();	

private:
	GLuint m_alpha;

}; // ShadowShader

}

#endif // _EASYSHADOW_SHADOW_SHADER_H_