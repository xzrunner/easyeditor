#ifndef _EASYEDITOR_SNAPSHOOT_H_
#define _EASYEDITOR_SNAPSHOOT_H_

#include <string>

namespace ee
{

class Symbol;
class Sprite;
class Shape;
class RenderTarget;

class Snapshoot
{
public:
	Snapshoot();
	Snapshoot(int width, int height);
	~Snapshoot();

	unsigned char* OutputToMemory(const Symbol* sym, bool whitebg = false,
		float scale = 1.0f) const;
	void OutputToImageFile(const Symbol* sym, const std::string& filename, 
		float scale = 1.0f) const;

	void DrawSprite(const Sprite* spr, bool clear = false,
		float dx = 0, float dy = 0) const;
	void DrawSprite(const Sprite* spr, bool clear, int width, int height,
		float dx = 0, float dy = 0, float scale = 1) const;

	void DrawShape(const Shape* shape, bool clear = false) const;

	void SaveToFile(const std::string& filename) const;
	void SaveToFile(const std::string& filename, int width, int height) const;

	const RenderTarget* GetFBO() const { return m_fbo; }

private:
	int m_width, m_height;

	RenderTarget* m_fbo;

}; // Snapshoot

}

#endif // _EASYEDITOR_SNAPSHOOT_H_