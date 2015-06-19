#ifndef _DRAG2D_OUTLINE_SHADER_H_
#define _DRAG2D_OUTLINE_SHADER_H_

#include "SpriteShader.h"

namespace d2d
{

class OutlineShader : public SpriteShader
{
protected:
	virtual void LoadShader();

}; // OutlineShader

}

#endif // _DRAG2D_OUTLINE_SHADER_H_