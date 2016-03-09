#ifndef _EASYEDITOR_STAGE_CANVAS_H_
#define _EASYEDITOR_STAGE_CANVAS_H_

#include "Screen.h"
#include "Color.h"
#include "Object.h"
#include "Observer.h"

#include <wx/glcanvas.h>
#include <wx/timer.h>

namespace ee
{

class EditPanelImpl;
class RenderContext;
class Camera;

class StageCanvas : public wxGLCanvas, public Object, public Observer
{
public:
	StageCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, wxGLContext* glctx = NULL,
		bool use_context_stack = true);
	virtual ~StageCanvas();

	void SetBgColor(const Colorf& color);

	void SetCurrentCanvas();

	Camera* GetCamera() { return m_camera; }
	const Camera* GetCamera() const { return m_camera; }

	bool IsDirty() const { return m_dirty; }

	void SetCamDirty() { m_cam_dirty = true; }

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	wxGLContext* GetGLContext() const { return m_gl_context; }

protected:
	virtual void OnSize(int w, int h) = 0;
	virtual void OnDrawWhole() const = 0;
	virtual void OnDrawSprites() const = 0;
	//virtual void OnDrawDC() const {}

	virtual void OnTimer() {}

	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

	void Init();

private:
	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);
	//void OnEraseBackground(wxEraseEvent& event);
	void OnMouse(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);

	void OnTimer(wxTimerEvent& event);

	void OnKillFocus(wxFocusEvent& event);

protected:
	EditPanelImpl* m_stage;

	Colorf m_bg_color;

	int m_width, m_height;

	Screen m_screen;
	Camera* m_camera;

private:
	enum
	{
		TIMER_ID = 9999
	};

private:
	bool m_share_context;
	bool m_use_context_stack;

	wxGLContext* m_gl_context;
	RenderContext* m_render_context;

	bool m_dirty;
	bool m_cam_dirty;

	wxTimer m_timer;

	int m_version;

	DECLARE_EVENT_TABLE()

}; // StageCanvas

}

#endif // _EASYEDITOR_STAGE_CANVAS_H_