#include "ReliefShader.h"

#define STRINGIFY(A)  #A
#include "NormalSpriteShader.vert"
#include "ReliefShader.frag"

#include <string>

namespace eimage
{

ReliefShader::ReliefShader()
{

}

void ReliefShader::LoadShader()
{
	static const std::string header(FLOAT_PRECISION);
	static const std::string vert(header + NormalVertShader);
	static const std::string frag(header + ReliefFragShader);

	InitShader(vert.c_str(), frag.c_str());
}

}