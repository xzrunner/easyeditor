#ifndef _EASYIMAGE_RELIEF_SHADER_H_
#define _EASYIMAGE_RELIEF_SHADER_H_

#include <drag2d.h>

namespace eimage
{

class ReliefShader : public d2d::SpriteShader
{
public:
	ReliefShader();

protected:
	virtual void LoadShader();

}; // ReliefShader

}

#endif // _EASYIMAGE_RELIEF_SHADER_H_