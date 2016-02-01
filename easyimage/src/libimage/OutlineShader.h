#ifndef _EASYIMAGE_OUTLINE_SHADER_H_
#define _EASYIMAGE_OUTLINE_SHADER_H_

#include <ee/SpriteShader.h>

namespace eimage
{

class OutlineShader : public ee::SpriteShader
{
protected:
	virtual void LoadShader();

}; // OutlineShader

}

#endif // _EASYIMAGE_OUTLINE_SHADER_H_