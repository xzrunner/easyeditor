#include <gl/glew.h>

#include "BlurShader.h"

#define STRINGIFY(A)  #A
#include "NormalSpriteShader.vert"
#include "BlurShader.frag"

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
	InitShader(NormalVertShader, BlurFragShader);

	m_radius = glGetUniformLocation(m_prog, "u_radius");
}

}