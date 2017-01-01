#ifndef _EASYEDITOR_CAMERA_CANVAS_H_
#define _EASYEDITOR_CAMERA_CANVAS_H_

#include "TwoPassCanvas.h"

#include <SM_Rect.h>
#include <gum/CameraType.h>

namespace gum { class Camera; }

namespace ee
{

class CameraCanvas : public TwoPassCanvas
{
public:
	CameraCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, gum::CameraType cam_type,
		wxGLContext* glctx = NULL, bool use_context_stack = true);
	virtual ~CameraCanvas();

protected:
	sm::rect GetVisibleRegion() const;

private:
	gum::Camera* m_camera;

}; // CameraCanvas

}

#endif // _EASYEDITOR_CAMERA_CANVAS_H_