#ifndef _EASYCOMPLEX3D_STAGE_CANVAS_H_
#define _EASYCOMPLEX3D_STAGE_CANVAS_H_

#include <ee/MultiSpritesImpl.h>

namespace ee { class LibraryPanel; }

namespace ecomplex3d
{

class StageCanvas : public e3d::StageCanvas
{
public:
	StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
		ee::MultiSpritesImpl* sprites_impl, ee::LibraryPanel* library);

	sm::ivec2 TransPos3ProjectToScreen(const sm::vec3& proj) const;
	sm::vec3 TransPos3ScreenToProject(const sm::ivec2& scr, float proj_z) const;

	sm::vec3 TransPos3ScreenToDir(const sm::ivec2& screen) const;

protected:
	virtual void OnSize(int w, int h) override;
	virtual void OnDrawSprites() const override;

private:
	void DrawBackground() const;
	void DrawSprites() const;

private:
	ee::MultiSpritesImpl* m_sprites_impl;
	ee::LibraryPanel* m_library;

	int m_screen_width, m_screen_height;

}; // StageCanvas

}

#endif // _EASYCOMPLEX3D_STAGE_CANVAS_H_
