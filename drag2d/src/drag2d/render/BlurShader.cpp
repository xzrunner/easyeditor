#include <gl/glew.h>

#include "BlurShader.h"

#define STRINGIFY(A)  #A
#include "SpriteShader.vert"
#include "BlurShader.frag"

namespace d2d
{

BlurShader::BlurShader()
	: m_radius(0)
{
}

void BlurShader::Bind()
{
	SpriteShader::Bind();
	SetRadius(2.0f);
}

void BlurShader::SetRadius(float r)
{
	glUniform1f(m_radius, r);
}

void BlurShader::LoadShader()
{
	InitShader(SpriteVertShader, BlurFragShader);

	m_radius = glGetUniformLocation(m_prog, "u_radius");
}

}