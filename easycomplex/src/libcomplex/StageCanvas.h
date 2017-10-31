#ifndef _EASYCOMPLEX_STAGE_CANVAS_H_
#define _EASYCOMPLEX_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>
#include <ee/FPS.h>
#include <ee/Symbol.h>

namespace ee { class LibraryPanel; }

namespace ecomplex
{

class StagePanel;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* editPanel, ee::LibraryPanel* library, 
		wxGLContext* glctx = NULL);
	
	void SetBackground(const ee::SymPtr& sym) { m_background = sym; }

protected:
	virtual void OnSize(int w, int h);
	virtual void OnDrawSprites() const;

protected:
	StagePanel* m_stage;

private:
	void DrawBackground() const;

	void DrawStat() const;

private:
	ee::LibraryPanel* m_library;

	ee::SymPtr m_background;

	mutable ee::FPS m_fps;

//	ee::Camera m_camera3;

}; // StageCanvas

}

#endif // _EASYCOMPLEX_STAGE_CANVAS_H_