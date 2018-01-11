#ifndef _EASYEDITOR_STAGE_CANVAS_H_
#define _EASYEDITOR_STAGE_CANVAS_H_

#include "Observer.h"

#include <sprite2/Color.h>
#include <cu/CU_RefCountObj.h>

#include <wx/glcanvas.h>
#include <wx/timer.h>

namespace ee
{

class EditPanelImpl;
class RenderContext;

class StageCanvas : public wxGLCanvas, public Observer, public cu::RefCountObj
{
public:
	static const uint32_t USE_CONTEXT_STACK = 0x00000001;
	static const uint32_t HAS_2D            = 0x00000002;
	static const uint32_t HAS_3D            = 0x00000004;

public:
	StageCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, wxGLContext* glctx = nullptr, 
		uint32_t flag = USE_CONTEXT_STACK | HAS_2D);
	virtual ~StageCanvas();

	virtual void SetCurrentCanvas();

	void SetBgColor(const s2::Color& color);

	bool IsDirty() const { return m_dirty; }

	void SetCamDirty() { m_cam_dirty = true; }

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	wxGLContext* GetGLContext() const { return m_gl_ctx; }

	void SetDrawable(bool draw) { m_draw = draw; }

protected:
	virtual void OnSize(int w, int h) = 0;
	virtual void OnDrawWhole() const = 0;
	virtual void OnDrawSprites() const = 0;
	//virtual void OnDrawDC() const {}

	virtual void OnTimer() {}

	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud) override;

	void Init();

private:
	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);
	//void OnEraseBackground(wxEraseEvent& event);
	void OnMouse(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnChar(wxKeyEvent& event);

	void OnTimer(wxTimerEvent& event);

	void OnKillFocus(wxFocusEvent& event);

protected:
	EditPanelImpl* m_stage;

	s2::Color m_bg_color;

	int m_width, m_height;

private:
	enum
	{
		TIMER_ID = 9999
	};

private:
	uint32_t m_flag;

	bool m_share_context;

	wxGLContext* m_gl_ctx;
	int m_ctx_idx_2d, m_ctx_idx_3d;

	bool m_dirty;
	bool m_cam_dirty;

	wxTimer m_timer;

	bool m_draw;

	DECLARE_EVENT_TABLE()

}; // StageCanvas

}

#endif // _EASYEDITOR_STAGE_CANVAS_H_