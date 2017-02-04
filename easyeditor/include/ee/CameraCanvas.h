#ifndef _EASYEDITOR_CAMERA_CANVAS_H_
#define _EASYEDITOR_CAMERA_CANVAS_H_

#include "TwoPassCanvas.h"

#include <SM_Rect.h>

namespace s2 { enum CameraType; class Camera; }

namespace ee
{

class CameraCanvas : public TwoPassCanvas
{
public:
	CameraCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, s2::CameraType cam_type,
		wxGLContext* glctx = NULL, bool use_context_stack = true);
	virtual ~CameraCanvas();

	s2::Camera* GetCamera() { return m_camera; }

protected:
	sm::rect GetVisibleRegion() const;
	float GetCameraScale() const;

protected:
	s2::Camera* m_camera;

}; // CameraCanvas

}

#endif // _EASYEDITOR_CAMERA_CANVAS_H_