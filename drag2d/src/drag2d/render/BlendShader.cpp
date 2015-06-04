#include "BlendShader.h"

#define STRINGIFY(A)  #A
#include "SpriteShader.vert"
#include "BlendShader.frag"

#include <gl/glew.h>
#include <string>

namespace d2d
{

BlendShader::BlendShader()
	: m_mode(BM_UNKNOWN)
	, m_mode_id(0)
{
}

void BlendShader::SetBaseTexID(int tex)
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex);
	glActiveTexture(GL_TEXTURE0);
}

void BlendShader::SetBlendMode(const std::string& str)
{
	BlendMode mode = BlendModes::Instance()->GetIDFromNameEN(str);
	if (mode == BM_UNKNOWN) {
		mode = BM_NORMAL;
	}

	if (mode != m_mode)
	{
		Commit();
		m_mode = mode;
		glUniform1i(m_mode_id, GLint(m_mode));
	}
}

void BlendShader::BindAttrib(GLuint prog)
{
	SpriteShader::BindAttrib(prog);
	m_mode_id = glGetUniformLocation(m_prog, "u_mode");
}

void BlendShader::LoadShader()
{
	static const std::string header(FLOAT_PRECISION);
	static const std::string vert(header + SpriteVertShader);
	static const std::string frag(header + BlendFragShader);

	InitShader(vert.c_str(), frag.c_str());
}

}