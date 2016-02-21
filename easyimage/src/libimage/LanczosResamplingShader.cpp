#include <gl/glew.h>

#include "LanczosResamplingShader.h"

#define STRINGIFY(A)  #A
#include "LanczosResamplingShader.vert"
#include "LanczosResamplingShader.frag"

#include <string>

namespace eimage
{

LanczosResamplingShader::LanczosResamplingShader()
{
	m_texel_width = m_texel_height = 0;
}

void LanczosResamplingShader::SetTexelSize(float width, float height)
{
 	glUniform1f(m_texel_width, width);
 	glUniform1f(m_texel_height, height);
}

void LanczosResamplingShader::LoadShader()
{
	static const std::string header(FLOAT_PRECISION);
	static const std::string vert(header + LanczosVertShader);
	static const std::string frag(header + LanczosFragShader);

	InitShader(vert.c_str(), frag.c_str());

  	m_texel_width = glGetUniformLocation(m_prog, "u_texel_width");
  	m_texel_height = glGetUniformLocation(m_prog, "u_texel_height");
}

}