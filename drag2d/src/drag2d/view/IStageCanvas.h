#ifndef _DRAG2D_INTERFACE_STAGE_CANVAS_H_
#define _DRAG2D_INTERFACE_STAGE_CANVAS_H_

#include <wx/glcanvas.h>
#include <wx/timer.h>

#include "Screen.h"
#include "common/Color.h"
#include "common/Object.h"
#include "message/Observer.h"

#include <vector>

namespace d2d
{

class EditPanelImpl;
class RenderContext;

class IStageCanvas : public wxGLCanvas, public Object, public Observer
{
public:
	IStageCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, wxGLContext* glctx = NULL);
	virtual ~IStageCanvas();

	void SetBgColor(const Colorf& color);

	void SetCurrentCanvas();

	Camera* GetCamera() { return m_camera; }
	const Camera* GetCamera() const { return m_camera; }

	bool IsDirty() const { return m_dirty; }

	void SetCamDirty() { m_cam_dirty = true; }

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	wxGLContext* GetGLContext() { return m_gl_context; }

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

	wxGLContext* m_gl_context;
	RenderContext* m_render_context;

	bool m_dirty;
	bool m_cam_dirty;

	wxTimer m_timer;

	int m_version;

	DECLARE_EVENT_TABLE()

}; // IStageCanvas

}

#endif // _DRAG2D_INTERFACE_STAGE_CANVAS_H_