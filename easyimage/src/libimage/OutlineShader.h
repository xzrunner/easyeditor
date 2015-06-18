#ifndef _EASYIMAGE_OUTLINE_SHADER_H_
#define _EASYIMAGE_OUTLINE_SHADER_H_

#include <drag2d.h>

namespace eimage
{

class OutlineShader : public d2d::SpriteShader
{
protected:
	virtual void LoadShader();

}; // OutlineShader

}

#endif // _EASYIMAGE_OUTLINE_SHADER_H_