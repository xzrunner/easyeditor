#ifndef _DRAG2D_STAGE_CANVAS_H_
#define _DRAG2D_STAGE_CANVAS_H_

#include <drag2d.h>
#include <easy3d.h>

namespace ecomplex
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* editPanel, d2d::LibraryPanel* library, 
		wxGLContext* glctx = NULL);
	
	virtual ~StageCanvas();

	void SetBackground(d2d::Symbol* symbol);

protected:
	virtual void OnSize(int w, int h);
	virtual void OnDrawSprites() const;

protected:
	StagePanel* m_stage;

private:
	void drawBackground() const;

private:
	d2d::LibraryPanel* m_library;

	d2d::ShapeStyle m_bgStyle, m_clipboxStyle;

	d2d::Symbol* m_background;

	mutable d2d::FPS m_fps;

	e3d::Camera m_camera3;

}; // StageCanvas

}

#endif // _DRAG2D_STAGE_CANVAS_H_