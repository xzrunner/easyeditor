#include "ReliefShader.h"

#define STRINGIFY(A)  #A
#include "SpriteShader.vert"
#include "ReliefShader.frag"

namespace d2d
{

ReliefShader::ReliefShader()
{

}

void ReliefShader::LoadShader()
{
	InitShader(SpriteVertShader, ReliefFragShader);
}

}