#ifndef _EASYEJOY2D_SCREEN_H_
#define _EASYEJOY2D_SCREEN_H_

#include <drag2d.h>

typedef unsigned int GLuint;

namespace eejoy2d
{

class EJScreen
{
public:
	void Load();
	void Unload();

	void Bind();
	void UnBind();

	GLuint GetTexID() const { return m_tex; }

public:
	static EJScreen* Instance();

private:
	EJScreen();
	~EJScreen();

	void InitRes();

private:
	GLuint m_tex, m_fbo;

private:
	static EJScreen* m_instance;

}; // EJScreen

}

#endif // _EASYEJOY2D_SCREEN_H_