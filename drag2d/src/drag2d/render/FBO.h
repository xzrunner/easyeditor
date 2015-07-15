#ifndef _DRAG2D_FBO_H_
#define _DRAG2D_FBO_H_

#include "common/Object.h"

#include <string>

typedef unsigned int GLuint;

namespace d2d
{

class ISprite;
class IShape;
class ISymbol;

class FBO : public Object
{
public:
	FBO();
	FBO(int width, int height);
	~FBO();

	void ChangeSize(int width, int height);

	void DrawSprite(const ISprite* sprite, bool clear = false,
		float dx = 0, float dy = 0);
	void DrawSprite(const ISprite* sprite, bool clear, int width, int height,
		float dx = 0, float dy = 0);
	void DrawShape(const IShape* shape, bool clear, int width, int height);
	void DrawSymbol(const ISymbol* symbol, bool whitebg = false,
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

	void DrawFBO(const ISymbol* symbol, bool whitebg = false,
		float scale = 1.0f);
	void DrawFBO(const ISprite* sprite, bool clear, int width, int height, 
		float dx, float dy);
	void DrawFBO(const IShape* shape, bool clear, int width, int height);

private:
	int m_width, m_height;

	GLuint m_tex;
	GLuint m_fbo;

}; // FBO

}

#endif // _DRAG2D_FBO_H_