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
	BlendMode mode = BM_NORMAL;
	// normal
	if (str == "normal") {
		mode = BM_NORMAL;
	} else if (str == "dissolve") {
		mode = BM_DISSOLVE;
	}
	// darken modes
	else if (str == "darken") {
		mode = BM_DARKEN;
	} else if (str == "multiply") {
		mode = BM_MULTIPLY;
	} else if (str == "color burn") {
		mode = BM_COLOR_BURN;
	} else if (str == "linear burn") {
		mode = BM_LINEAR_BURN;
	} else if (str == "darker color") {
		mode = BM_DARKER_COLOR;
	}
	// lighten modes
	else if (str == "lighten") {
		mode = BM_LIGHTEN;
	} else if (str == "screen") {
		mode = BM_SCREEN;
	} else if (str == "color dodge") {
		mode = BM_COLOR_DODGE;
	} else if (str == "linear dodge") {
		mode = BM_LINEAR_DODGE;
	} else if (str == "lighter color") {
		mode = BM_LIGHTER_COLOR;
	}
	// saturation modes
	else if (str == "overlay") {
		mode = BM_OVERLAY;
	} else if (str == "soft light") {
		mode = BM_SOFT_LIGHT;
	} else if (str == "hard light") {
		mode = BM_HARD_LIGHT;
	} else if (str == "vivid light") {
		mode = BM_VIVID_LIGHT;
	} else if (str == "linear light") {
		mode = BM_LINEAR_LIGHT;
	} else if (str == "pin light") {
		mode = BM_PIN_LIGHT;
	} else if (str == "hard mix") {
		mode = BM_HARD_MIX;
	}
	// substraction modes
	else if (str == "difference") {
		mode = BM_DIFFERENCE;
	} else if (str == "exclusion") {
		mode = BM_EXCLUSION;
	}
	// color modes 
	else if (str == "hue") {
		mode = BM_HUE;
	} else if (str == "saturation") {
		mode = BM_SATURATION;
	} else if (str == "color") {
		mode = BM_COLOR;
	} else if (str == "luminosity") {
		mode = BM_LUMINOSITY;
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