#ifndef _DRAG2D_SCREEN_CACHE_H_
#define _DRAG2D_SCREEN_CACHE_H_

namespace d2d
{

class Camera;

class ScreenCache
{
public:
	void OnSize(int w, int h);

	void Reload();

	void Bind();
	void Unbind();

	void InvalidRect(float xmin, float ymin, float xmax, float ymax);
	void Draw(const Camera* cam);

	void DrawToScreen(void (*before_draw)(void* ud), void* ud);

	static ScreenCache* Instance();

private:
	ScreenCache();

private:
	static ScreenCache* m_instance;

}; // ScreenCache

}

#endif // _DRAG2D_SCREEN_CACHE_H_
