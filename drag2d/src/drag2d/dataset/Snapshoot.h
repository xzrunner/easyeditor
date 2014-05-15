#ifndef _D2D_SNAPSHOOT_H_
#define _D2D_SNAPSHOOT_H_

#include <string>

namespace anim { class Symbol; }

namespace d2d
{

class ISymbol;

class Snapshoot
{
public:
	Snapshoot();
	Snapshoot(int width, int height);
	~Snapshoot();

	unsigned char* outputToMemory(const ISymbol* symbol, bool whitebg = false) const;
	unsigned char* outputAnimToMemory(const ISymbol* symbol, int index, bool whitebg = false) const;

	void outputToImageFile(const ISymbol* symbol, const std::string& filename) const;

private:
	void createFBO(int width, int height);
	void releaseFBO();

	void drawFBO(const ISymbol* symbol, bool whitebg = false) const;
	void drawAnimFBO(const anim::Symbol* symbol, int index, bool whitebg = false) const;

	int checkFramebufferStatus() const;

private:
	static const int DEFAULT_WIDTH = 800;
	static const int DEFAULT_HEIGHT = 600;

private:
	int m_tex;
	int m_fbo;

}; // Snapshoot

}

#endif // _D2D_SNAPSHOOT_H_