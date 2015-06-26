#include <gl/glew.h>

#include "BlurShader.h"

#define STRINGIFY(A)  #A
#include "NormalSpriteShader.vert"
#include "BlurShader.frag"

#include <string>

namespace eimage
{

BlurShader::BlurShader()
	: m_radius(0)
{
}

void BlurShader::SetRadius(float r)
{
	glUniform1f(m_radius, r);
}

void BlurShader::LoadShader()
{
	static const std::string header(FLOAT_PRECISION);
	static const std::string vert(header + NormalVertShader);
	static const std::string frag(header + BlurFragShader);

	InitShader(vert.c_str(), frag.c_str());

	m_radius = glGetUniformLocation(m_prog, "u_radius");
}

}