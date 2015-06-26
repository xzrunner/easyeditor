#include "OutlineShader.h"

#define STRINGIFY(A)  #A
#include "NormalSpriteShader.vert"
#include "OutlineShader.frag"

#include <string>

namespace eimage
{

void OutlineShader::LoadShader()
{
	static const std::string header(FLOAT_PRECISION);
	static const std::string vert(header + NormalVertShader);
	static const std::string frag(header + OutlineFragShader);

	InitShader(vert.c_str(), frag.c_str());
}

}