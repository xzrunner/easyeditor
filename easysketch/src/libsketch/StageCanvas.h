#ifndef _LIBSKETCH_STAGE_CANVAS_H_
#define _LIBSKETCH_STAGE_CANVAS_H_

#include <drag2d.h>
#include <easy3d.h>

namespace libsketch
{

class StagePanel;

class StageCanvas : public e3d::StageCanvas
{
public:
	StageCanvas(StagePanel* stage, d2d::LibraryPanel* library);

	ivec2 TransPos3ProjectToScreen(const vec3& proj) const;
	vec3 TransPos3ScreenToProject(const ivec2& scr, float proj_z) const;

	vec3 TransPos3ScreenToDir(const ivec2& screen) const;

protected:
	virtual void initGL();
	virtual void onSize(int w, int h);
	virtual void onDraw();

private:
	void InitDefaultSymbol();

	void LoadSymbolToLib(e3d::ISurface* surface, const char* name);

	void DrawBackground() const;
	void DrawSprites() const;

private:
	StagePanel* m_stage;
	d2d::LibraryPanel* m_library;

	int m_screen_width, m_screen_height;

}; // StageCanvas

}

#endif // _LIBSKETCH_STAGE_CANVAS_H_
