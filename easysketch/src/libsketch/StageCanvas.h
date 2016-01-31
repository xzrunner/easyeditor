#ifndef _LIBSKETCH_STAGE_CANVAS_H_
#define _LIBSKETCH_STAGE_CANVAS_H_


#include <easy3d.h>

namespace libsketch
{

class StageCanvas : public e3d::StageCanvas
{
public:
	StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
		ee::MultiSpritesImpl* sprites_impl, ee::LibraryPanel* library);

	ivec2 TransPos3ProjectToScreen(const vec3& proj) const;
	vec3 TransPos3ScreenToProject(const ivec2& scr, float proj_z) const;

	vec3 TransPos3ScreenToDir(const ivec2& screen) const;

protected:
	virtual void OnSize(int w, int h);
	virtual void OnDrawSprites() const;

private:
	void DrawBackground() const;
	void DrawSprites() const;

private:
	ee::MultiSpritesImpl* m_sprites_impl;
	ee::LibraryPanel* m_library;

	int m_screen_width, m_screen_height;

}; // StageCanvas

}

#endif // _LIBSKETCH_STAGE_CANVAS_H_
