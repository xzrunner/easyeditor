#ifndef _DRAG2D_STAGE_CANVAS_H_
#define _DRAG2D_STAGE_CANVAS_H_


#include <easy3d.h>

namespace ecomplex
{

class StagePanel;

class StageCanvas : public ee::OrthoCanvas
{
public:
	StageCanvas(StagePanel* editPanel, ee::LibraryPanel* library, 
		wxGLContext* glctx = NULL);
	
	virtual ~StageCanvas();

	void SetBackground(ee::Symbol* symbol);

protected:
	virtual void OnSize(int w, int h);
	virtual void OnDrawSprites() const;

protected:
	StagePanel* m_stage;

private:
	void drawBackground() const;

private:
	ee::LibraryPanel* m_library;

	ee::ShapeStyle m_bgStyle, m_clipboxStyle;

	ee::Symbol* m_background;

	mutable ee::FPS m_fps;

	e3d::Camera m_camera3;

}; // StageCanvas

}

#endif // _DRAG2D_STAGE_CANVAS_H_