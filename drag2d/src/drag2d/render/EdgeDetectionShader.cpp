#include <gl/glew.h>

#include "EdgeDetectionShader.h"

#define STRINGIFY(A)  #A
#include "EdgeDetectionShader.vert"
#include "EdgeDetectionShader.frag"

namespace d2d
{

EdgeDetectionShader::EdgeDetectionShader()
{
	m_blend = 0;
}

void EdgeDetectionShader::Bind()
{
	SpriteShader::Bind();
	SetBlend(1);
}

void EdgeDetectionShader::SetBlend(float blend)
{
	glUniform1f(m_blend, blend);
}

void EdgeDetectionShader::LoadShader()
{
	InitShader(EdgeDetectionVertShader, EdgeDetectionFragShader);

	m_blend = glGetUniformLocation(m_prog, "u_blend");
}

}