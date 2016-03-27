#ifndef _EASYEDITOR_CAMERA_CANVAS_H_
#define _EASYEDITOR_CAMERA_CANVAS_H_

#include "TwoPassCanvas.h"

namespace ee
{

class CameraCanvas : public TwoPassCanvas
{
public:
	CameraCanvas(wxWindow* stage_wnd, EditPanelImpl* stage,
		wxGLContext* glctx = NULL, bool use_context_stack = true);

protected:
	virtual void OnSize(int w, int h);

}; // CameraCanvas

}

#endif // _EASYEDITOR_CAMERA_CANVAS_H_