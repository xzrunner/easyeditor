#include "ScreenFBO.h"

namespace d2d
{

ScreenFBO* ScreenFBO::m_instance = NULL;

void ScreenFBO::ChangeSize(int width, int height)
{
	m_fbo.ChangeSize(width, height);
}

GLuint ScreenFBO::GetTexID() const
{
	return m_fbo.GetTexID();
}

GLuint ScreenFBO::GetFboID() const
{
	return m_fbo.GetFboID();
}

ScreenFBO* ScreenFBO::Instance()
{
	if (!m_instance) {
		m_instance = new ScreenFBO();
	}
	return m_instance;
}

}