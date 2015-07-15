#ifndef _D2D_SNAPSHOOT_H_
#define _D2D_SNAPSHOOT_H_

#include <string>

namespace d2d
{

class ISymbol;
class ISprite;
class IShape;
class FBO;

class Snapshoot
{
public:
	Snapshoot();
	Snapshoot(int width, int height);
	~Snapshoot();

	unsigned char* OutputToMemory(const ISymbol* symbol, bool whitebg = false,
		float scale = 1.0f) const;
	void OutputToImageFile(const ISymbol* symbol, const std::string& filename, 
		float scale = 1.0f) const;

	void DrawSprite(const ISprite* sprite, bool clear = false,
		float dx = 0, float dy = 0) const;
	void DrawSprite(const ISprite* sprite, bool clear, int width, int height,
		float dx = 0, float dy = 0) const;

	void DrawShape(const IShape* shape, bool clear = false) const;

	void SaveToFile(const std::string& filename) const;
	void SaveToFile(const std::string& filename, int width, int height) const;

	const FBO* GetFBO() const { return m_fbo; }

private:
	int m_width, m_height;

	FBO* m_fbo;

}; // Snapshoot

}

#endif // _D2D_SNAPSHOOT_H_