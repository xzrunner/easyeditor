//#include <gl/glew.h>
//
//#include "ShadowShader.h"
//
//#define STRINGIFY(A)  #A
//#include "ShadowShader.vert"
//#include "ShadowShader.frag"
//
//#include <string>
//
//namespace eshadow
//{
//
//ShadowShader::ShadowShader()
//{
//	m_alpha = 0;
//}
//
//void ShadowShader::SetAlpha(float alpha)
//{
//	glUniform1f(m_alpha, alpha);
//}
//
//void ShadowShader::LoadShader()
//{
//	static const std::string header(FLOAT_PRECISION);
//	static const std::string vert(header + ShadowVertShader);
//	static const std::string frag(header + ShadowFragShader);
//
//	InitShader(vert.c_str(), frag.c_str());
//
//	m_alpha = glGetUniformLocation(m_prog, "u_alpha");
//}
//
//}