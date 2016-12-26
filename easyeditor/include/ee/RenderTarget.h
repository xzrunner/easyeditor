#ifndef _EASYEDITOR_RENDER_TARGET_H_
#define _EASYEDITOR_RENDER_TARGET_H_

#include <CU_RefCountObj.h>

#include <string>

namespace ur { class RenderTarget; }

namespace ee
{

class Sprite;
class Shape;
class Symbol;

class RenderTarget : public cu::RefCountObj
{
public:
	RenderTarget();
	RenderTarget(int width, int height);
	~RenderTarget();

	void ChangeSize(int width, int height);

	void DrawSprite(const Sprite* spr, bool clear = false,
		float dx = 0, float dy = 0);
	void DrawSprite(const Sprite* spr, bool clear, int width, int height,
		float dx = 0, float dy = 0, float scale = 1);
	void DrawShape(const Shape* shape, bool clear, int width, int height);
	void DrawSymbol(const Symbol* sym, bool whitebg = false,
		float scale = 1.0f);

	void ReadPixels(unsigned char* pixels, int width, int height) const;

	int GetTexID() const;
//	int GetFboID() const;

	int GetWidth() const;
	int GetHeight() const;

private:
	void DrawFBO(const Symbol* sym, bool whitebg = false,
		float scale = 1.0f);
	void DrawFBO(const Sprite* spr, bool clear, int width, int height, 
		float dx, float dy, float scale);
	void DrawFBO(const Shape* shape, bool clear, int width, int height);

private:
	ur::RenderTarget* m_impl;

}; // RenderTarget

}

#endif // _EASYEDITOR_RENDER_TARGET_H_