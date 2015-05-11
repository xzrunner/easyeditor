#ifndef _D2D_SNAPSHOOT_H_
#define _D2D_SNAPSHOOT_H_

#include <string>

namespace libanim { class Symbol; }

//#define TEST_RESAMPLING

namespace d2d
{

class ISymbol;
class ISprite;

class SpriteShader;

class Snapshoot
{
public:
	Snapshoot();
	Snapshoot(int width, int height);
	~Snapshoot();

	unsigned char* outputToMemory(const ISymbol* symbol, bool whitebg = false,
		float scale = 1.0f) const;

	void outputToImageFile(const ISymbol* symbol, const std::string& filename, 
		float scale = 1.0f) const;

	void DrawSprite(const ISprite* sprite, bool clear = false) const;
	void SaveToFile(const std::string& filename) const;

	void DrawSprite(const ISprite* sprite, bool clear, int width, int height) const;
	void SaveToFile(const std::string& filename, int width, int height) const;

private:
	void createFBO();
	void releaseFBO();

	void drawFBO(const ISymbol* symbol, bool whitebg = false,
		float scale = 1.0f) const;
	void drawFBO(const ISprite* sprite, bool clear, int width, int height) const;

	int checkFramebufferStatus() const;

	void ReadPixels(unsigned char* pixels, int width, int height) const;

private:
	int m_width, m_height;

	typedef unsigned int GLuint;
	GLuint m_tex;
	GLuint m_fbo;

#ifdef TEST_RESAMPLING
	SpriteShader *m_old_shader, *m_new_shader;
#endif

}; // Snapshoot

}

#endif // _D2D_SNAPSHOOT_H_