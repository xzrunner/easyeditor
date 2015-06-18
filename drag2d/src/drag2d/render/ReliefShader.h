#ifndef _DRAG2D_RELIEF_SHADER_H_
#define _DRAG2D_RELIEF_SHADER_H_

#include "SpriteShader.h"

namespace d2d
{

class ReliefShader : public SpriteShader
{
public:
	ReliefShader();

protected:
	virtual void LoadShader();

}; // ReliefShader

}

#endif // _DRAG2D_RELIEF_SHADER_H_