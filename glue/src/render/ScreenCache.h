#ifndef _GLUE_SCREEN_CACHE_H_
#define _GLUE_SCREEN_CACHE_H_

#include "utility/macro.h"

struct dtex_cs;

namespace glue
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

private:
	int m_width, m_height;

	dtex_cs* m_cs;

private:
	SINGLETON_DECLARATION(ScreenCache)

}; // ScreenCache

}

#endif // _GLUE_SCREEN_CACHE_H_