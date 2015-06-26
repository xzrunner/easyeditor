#include <gl/glew.h>

#include "EdgeDetectionShader.h"

#define STRINGIFY(A)  #A
#include "NormalSpriteShader.vert"
#include "EdgeDetectionShader.frag"

#include <string>

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
	static const std::string header(FLOAT_PRECISION);
	static const std::string vert(header + NormalVertShader);
	static const std::string frag(header + EdgeDetectionFragShader);

	InitShader(vert.c_str(), frag.c_str());

	m_blend = glGetUniformLocation(m_prog, "u_blend");
}

}