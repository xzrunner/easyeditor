#include "OutlineShader.h"

#define STRINGIFY(A)  #A
#include "SpriteShader.vert"
#include "OutlineShader.frag"

namespace d2d
{

void OutlineShader::LoadShader()
{
	InitShader(SpriteVertShader, OutlineFragShader);
}

}