#include "ShadowShader.h"

#define STRINGIFY(A)  #A
#include "ShadowShader.vert"
#include "ShadowShader.frag"

namespace eshadow
{

void ShadowShader::LoadShader()
{
	static const std::string header(FLOAT_PRECISION);
	static const std::string vert(header + ShadowVertShader);
	static const std::string frag(header + ShadowFragShader);

	InitShader(vert.c_str(), frag.c_str());
}

}