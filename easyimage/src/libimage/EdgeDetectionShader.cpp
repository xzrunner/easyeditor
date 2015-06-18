#include <gl/glew.h>

#include "EdgeDetectionShader.h"

#define STRINGIFY(A)  #A
#include "NormalSpriteShader.vert"
#include "EdgeDetectionShader.frag"

namespace eimage
{

EdgeDetectionShader::EdgeDetectionShader()
{
	m_blend = 0;
}

void EdgeDetectionShader::SetBlend(float blend)
{
	glUniform1f(m_blend, blend);
}

void EdgeDetectionShader::LoadShader()
{
	InitShader(NormalVertShader, EdgeDetectionFragShader);

	m_blend = glGetUniformLocation(m_prog, "u_blend");
}

}