//#ifndef _EASYEDITOR_RENDER_TARGET_H_
//#define _EASYEDITOR_RENDER_TARGET_H_
//
//#include "Object.h"
//
//namespace ee
//{
//
//class RenderTarget : public Object
//{
//public:
//	RenderTarget();
//	RenderTarget(int width, int height);
//	~RenderTarget();
//
//	void Reize(int width, int height);
//
//	void DrawSprite(const Sprite* sprite, bool clear = false,
//		float dx = 0, float dy = 0);
//	void DrawSprite(const Sprite* sprite, bool clear, int width, int height,
//		float dx = 0, float dy = 0);
//	void DrawShape(const Shape* shape, bool clear, int width, int height);
//	void DrawSymbol(const Symbol* symbol, bool whitebg = false,
//		float scale = 1.0f);
//
//	void ReadPixels(unsigned char* pixels, int width, int height) const;
//
//	int GetWidth() const { return m_width; }
//	int GetHeight() const { return m_height; }
//
//private:
//	static int Create(int w, int h);
//	static void Release(int id);
//
//	void DrawFBO(const Symbol* symbol, bool whitebg = false,
//		float scale = 1.0f);
//	void DrawFBO(const Sprite* sprite, bool clear, int width, int height, 
//		float dx, float dy);
//	void DrawFBO(const Shape* shape, bool clear, int width, int height);
//
//private:
//	int m_id;
//
//	int m_width, m_height;
//
//}; // RenderTarget
//
//}
//
//#endif // _EASYEDITOR_RENDER_TARGET_H_