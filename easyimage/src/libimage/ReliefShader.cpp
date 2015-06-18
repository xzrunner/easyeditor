#include "ReliefShader.h"

#define STRINGIFY(A)  #A
#include "NormalSpriteShader.vert"
#include "ReliefShader.frag"

namespace eimage
{

ReliefShader::ReliefShader()
{

}

void ReliefShader::LoadShader()
{
	InitShader(NormalVertShader, ReliefFragShader);
}

}