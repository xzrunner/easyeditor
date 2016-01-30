#ifndef _DRAG2D_TWO_PASS_CANVAS_H_
#define _DRAG2D_TWO_PASS_CANVAS_H_

#include "IStageCanvas.h"

namespace d2d
{

class TwoPassCanvas : public IStageCanvas
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

#endif // _DRAG2D_TWO_PASS_CANVAS_H_