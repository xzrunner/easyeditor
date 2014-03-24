#ifndef _D2D_SNAPSHOOT_H_
#define _D2D_SNAPSHOOT_H_

#include <string>

namespace d2d
{

class ISymbol;

class Snapshoot
{
public:
	Snapshoot();
	~Snapshoot();

	void outputToImageFile(const ISymbol* symbol, const std::string& filename) const;

private:
	void createFBO(int width, int height);
	void releaseFBO();

	void drawFBO(const ISymbol* symbol) const;

	int checkFramebufferStatus() const;

private:
	int m_tex;
	int m_fbo;

}; // Snapshoot

}

#endif // _D2D_SNAPSHOOT_H_