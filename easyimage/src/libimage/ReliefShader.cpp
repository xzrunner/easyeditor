#include "ReliefShader.h"

#define STRINGIFY(A)  #A
#include "ReliefShader.vert"
#include "ReliefShader.frag"

namespace eimage
{

ReliefShader::ReliefShader()
{

}

void ReliefShader::LoadShader()
{
	InitShader(ReliefVertShader, ReliefFragShader);
}

}