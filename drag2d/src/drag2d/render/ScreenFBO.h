#ifndef _DRAG2D_SCREEN_FBO_H_
#define _DRAG2D_SCREEN_FBO_H_

#include "FBO.h"

namespace d2d
{

class ScreenFBO
{
public:
	void ChangeSize(int width, int height);

	GLuint GetTexID() const;
	GLuint GetFboID() const;

	static ScreenFBO* Instance();

private:
	ScreenFBO() {}

private:
	FBO m_fbo;

private:
	static ScreenFBO* m_instance;

}; // ScreenFBO

}

#endif // _DRAG2D_SCREEN_FBO_H_