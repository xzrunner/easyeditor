#ifndef _EASYCOMPLEX_STAGE_CANVAS_H_
#define _EASYCOMPLEX_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>
#include <ee/FPS.h>
#include <ee/Camera.h>

namespace ee { class LibraryPanel; class Symbol; }

namespace ecomplex
{

class StagePanel;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* editPanel, ee::LibraryPanel* library, 
		wxGLContext* glctx = NULL);
	
	virtual ~StageCanvas();

	void SetBackground(ee::Symbol* sym);

protected:
	virtual void OnSize(int w, int h);
	virtual void OnDrawSprites() const;

protected:
	StagePanel* m_stage;

private:
	void DrawBackground() const;

private:
	ee::LibraryPanel* m_library;

	ee::Symbol* m_background;

	mutable ee::FPS m_fps;

//	ee::Camera m_camera3;

}; // StageCanvas

}

#endif // _EASYCOMPLEX_STAGE_CANVAS_H_