#ifndef _EASYIMAGE_RELIEF_SHADER_H_
#define _EASYIMAGE_RELIEF_SHADER_H_

#include <ee/SpriteShader.h>

namespace eimage
{

class ReliefShader : public ee::SpriteShader
{
public:
	ReliefShader();

protected:
	virtual void LoadShader();

}; // ReliefShader

}

#endif // _EASYIMAGE_RELIEF_SHADER_H_