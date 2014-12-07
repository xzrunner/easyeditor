#define NOMINMAX
#include <gl/GLee.h>

#include "LanczosResamplingShader.h"

#define STRINGIFY(A)  #A
#include "LanczosResamplingShader.vert"
#include "LanczosResamplingShader.frag"

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
 	InitShader(LanczosVertShader, LanczosFragShader);
 
  	m_texel_width = glGetUniformLocation(m_prog, "u_texel_width");
  	m_texel_height = glGetUniformLocation(m_prog, "u_texel_height");
}

}