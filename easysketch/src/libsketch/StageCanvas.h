#ifndef _LIBSKETCH_STAGE_CANVAS_H_
#define _LIBSKETCH_STAGE_CANVAS_H_

#include <drag2d.h>
#include <easy3d.h>

namespace z3d { class ISurface; }

namespace libsketch
{

class StagePanel;

class StageCanvas : public e3d::StageCanvas
{
public:
	StageCanvas(StagePanel* stage, d2d::LibraryPanel* library);

	ivec2 TransPos3ProjectToScreen(const vec3& proj) const;

protected:
	virtual void initGL();
	virtual void onSize(int w, int h);
	virtual void onDraw();

private:
	void InitDefaultSymbol();

	void LoadSymbolToLib(z3d::ISurface* surface, const char* name);

private:
	StagePanel* m_stage;
	d2d::LibraryPanel* m_library;

	int m_width, m_height;

}; // StageCanvas

}

#endif // _LIBSKETCH_STAGE_CANVAS_H_
