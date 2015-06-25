#ifndef _EASYSHADOW_SHADOW_SHADER_H_
#define _EASYSHADOW_SHADOW_SHADER_H_

#include <drag2d.h>

namespace eshadow
{

class ShadowShader : public d2d::ShapeShader
{
public:

protected:
	virtual void LoadShader();	

}; // ShadowShader

}

#endif // _EASYSHADOW_SHADOW_SHADER_H_