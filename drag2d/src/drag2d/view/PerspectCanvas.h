#ifndef _DRAG2D_PERSPECT_CANVAS_H_
#define _DRAG2D_PERSPECT_CANVAS_H_

#include "OnePassCanvas.h"

namespace d2d
{

class PerspectCanvas : public OnePassCanvas
{
public:
	PerspectCanvas(wxWindow* stage_wnd, EditPanelImpl* stage,
		wxGLContext* glctx = NULL);

private:
	virtual void OnSize(int w, int h);

}; // PerspectCanvas

}

#endif // _DRAG2D_PERSPECT_CANVAS_H_