#ifndef _DRAG2D_FBO_H_
#define _DRAG2D_FBO_H_

#include <string>

namespace d2d
{

class ISprite;
class ISymbol;

class FBO
{
public:
	FBO(int width, int height);
	~FBO();

	void DrawSprite(const ISprite* sprite, bool clear = false,
		float dx = 0, float dy = 0);
	void DrawSprite(const ISprite* sprite, bool clear, int width, int height,
		float dx = 0, float dy = 0);
	void DrawSymbol(const ISymbol* symbol, bool whitebg = false,
		float scale = 1.0f);

	void ReadPixels(unsigned char* pixels, int width, int height) const;

private:
	void CreateFBO(int w, int h);
	void ReleaseFBO();

	int CheckFramebufferStatus(std::string& msg) const;

	void DrawFBO(const ISymbol* symbol, bool whitebg = false,
		float scale = 1.0f);
	void DrawFBO(const ISprite* sprite, bool clear, int width, int height, 
		float dx, float dy);

private:
	int m_width, m_height;

	typedef unsigned int GLuint;
	GLuint m_tex;
	GLuint m_fbo;

}; // FBO

}

#endif // _DRAG2D_FBO_H_