#ifndef _DRAG2D_SCREEN_CACHE_H_
#define _DRAG2D_SCREEN_CACHE_H_

namespace d2d
{

class Camera;

class ScreenCache
{
public:
	void SetSize(int w, int h);
	void GetSize(int& w, int& h) const;

	void Reload();

	void Bind();
	void Unbind();

	void InvalidRect(float xmin, float ymin, float xmax, float ymax);
	void Draw(const Camera* cam);

	void DrawToScreen(void (*before_draw)(void* ud), void* ud);

	int GetTexID() const;

	static ScreenCache* Instance();

private:
	ScreenCache();

private:
	int m_width, m_height;

private:
	static ScreenCache* m_instance;

}; // ScreenCache

}

#endif // _DRAG2D_SCREEN_CACHE_H_
