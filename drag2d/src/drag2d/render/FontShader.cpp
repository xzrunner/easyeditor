#include "FontShader.h"

#define STRINGIFY(A)  #A
#include "SpriteShader.vert"
#include "FontShader.frag"

#include <string>

namespace d2d
{

FontShader::FontShader()
{

}

void FontShader::Load()	
{
	static const std::string header(FLOAT_PRECISION);
	static const std::string vert(header + SpriteVertShader);
	static const std::string frag(header + FontFragShader);

	InitShader(vert.c_str(), frag.c_str());
}

void FontShader::Unload()
{

}

}