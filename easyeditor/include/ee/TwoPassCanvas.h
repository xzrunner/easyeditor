#ifndef _EASYEDITOR_TWO_PASS_CANVAS_H_
#define _EASYEDITOR_TWO_PASS_CANVAS_H_

#include "StageCanvas.h"

namespace ee
{

class TwoPassCanvas : public StageCanvas
{
public:
	TwoPassCanvas(wxWindow* stage_wnd, EditPanelImpl* stage,
		wxGLContext* glctx = NULL, bool use_context_stack = true);

public:
	struct ScreenStyle
	{
		s2::Color multi_col, add_col;
		bool col_grading;

		ScreenStyle() : multi_col(0xffffffff), add_col(0), col_grading(false) {}
	};

protected:
	virtual void OnSize(int w, int h);
	virtual void OnDrawWhole() const;

private:
	void DrawOnePass() const;
	void DrawTwoPass() const;

	void DebugDraw() const;

protected:
	ScreenStyle m_scr_style;

}; // TwoPassCanvas

}

#endif // _EASYEDITOR_TWO_PASS_CANVAS_H_