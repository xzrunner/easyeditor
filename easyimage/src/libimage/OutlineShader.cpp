#include "OutlineShader.h"

#define STRINGIFY(A)  #A
#include "NormalSpriteShader.vert"
#include "OutlineShader.frag"

namespace eimage
{

void OutlineShader::LoadShader()
{
	InitShader(NormalVertShader, OutlineFragShader);
}

}