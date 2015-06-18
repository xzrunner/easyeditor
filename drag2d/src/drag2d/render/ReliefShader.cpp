#include "ReliefShader.h"

#define STRINGIFY(A)  #A
#include "ReliefShader.vert"
#include "ReliefShader.frag"

namespace d2d
{

ReliefShader::ReliefShader()
{

}

void ReliefShader::LoadShader()
{
	InitShader(ReliefVertShader, ReliefFragShader);
}

}