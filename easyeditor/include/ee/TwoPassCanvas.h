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

protected:
	virtual void OnSize(int w, int h);
	virtual void OnDrawWhole() const;

public:
	struct ScreenStyle
	{
		Colorf multi_col, add_col;
	};

protected:
	ScreenStyle m_scr_style;

}; // TwoPassCanvas

}

#endif // _EASYEDITOR_TWO_PASS_CANVAS_H_