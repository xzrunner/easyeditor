#ifndef _EASYEDITOR_FBO_H_
#define _EASYEDITOR_FBO_H_

#include "Object.h"

#include <string>

typedef unsigned int GLuint;

namespace ee
{

class Sprite;
class Shape;
class Symbol;

class FBO : public Object
{
public:
	FBO();
	FBO(int width, int height);
	~FBO();

	void ChangeSize(int width, int height);

	void DrawSprite(const Sprite* sprite, bool clear = false,
		float dx = 0, float dy = 0);
	void DrawSprite(const Sprite* sprite, bool clear, int width, int height,
		float dx = 0, float dy = 0);
	void DrawShape(const Shape* shape, bool clear, int width, int height);
	void DrawSymbol(const Symbol* symbol, bool whitebg = false,
		float scale = 1.0f);

	void ReadPixels(unsigned char* pixels, int width, int height) const;

	GLuint GetTexID() const { return m_tex; }
	GLuint GetFboID() const { return m_fbo; }

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

private:
	void CreateFBO(int w, int h);
	void ReleaseFBO();

	int CheckFramebufferStatus(std::string& msg) const;

	void DrawFBO(const Symbol* symbol, bool whitebg = false,
		float scale = 1.0f);
	void DrawFBO(const Sprite* sprite, bool clear, int width, int height, 
		float dx, float dy);
	void DrawFBO(const Shape* shape, bool clear, int width, int height);

private:
	int m_width, m_height;

	GLuint m_tex;
	GLuint m_fbo;

}; // FBO

}

#endif // _EASYEDITOR_FBO_H_