#ifndef _DRAG2D_SCREEN_FBO_H_
#define _DRAG2D_SCREEN_FBO_H_

#include "FBO.h"

namespace d2d
{

class ScreenFBO
{
public:
	const FBO& GetFBO() const { return m_fbo; }
	FBO& GetFBO() { return m_fbo; }

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