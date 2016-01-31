#ifndef _EASYEDITOR_PERSPECT_CANVAS_H_
#define _EASYEDITOR_PERSPECT_CANVAS_H_

#include "OnePassCanvas.h"

namespace ee
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

#endif // _EASYEDITOR_PERSPECT_CANVAS_H_