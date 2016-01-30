#ifndef _EASYEDITOR_SNAPSHOOT_H_
#define _EASYEDITOR_SNAPSHOOT_H_

#include <string>

namespace ee
{

class Symbol;
class Sprite;
class Shape;
class FBO;

class Snapshoot
{
public:
	Snapshoot();
	Snapshoot(int width, int height);
	~Snapshoot();

	unsigned char* OutputToMemory(const Symbol* symbol, bool whitebg = false,
		float scale = 1.0f) const;
	void OutputToImageFile(const Symbol* symbol, const std::string& filename, 
		float scale = 1.0f) const;

	void DrawSprite(const Sprite* sprite, bool clear = false,
		float dx = 0, float dy = 0) const;
	void DrawSprite(const Sprite* sprite, bool clear, int width, int height,
		float dx = 0, float dy = 0) const;

	void DrawShape(const Shape* shape, bool clear = false) const;

	void SaveToFile(const std::string& filename) const;
	void SaveToFile(const std::string& filename, int width, int height) const;

	const FBO* GetFBO() const { return m_fbo; }

private:
	int m_width, m_height;

	FBO* m_fbo;

}; // Snapshoot

}

#endif // _EASYEDITOR_SNAPSHOOT_H_