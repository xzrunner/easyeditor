#ifndef _EASYEDITOR_CAMERA_CANVAS_H_
#define _EASYEDITOR_CAMERA_CANVAS_H_

#include "TwoPassCanvas.h"

#include <SM_Rect.h>

#include <memory>

namespace s2 { enum CameraType; class Camera; }
namespace pt3 { class Camera; }

namespace ee
{

class CameraCanvas : public TwoPassCanvas
{
public:
	CameraCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, s2::CameraType cam_type,
		wxGLContext* glctx = NULL, bool use_context_stack = true, bool has_3d = false);
	virtual ~CameraCanvas();

	virtual void SetCurrentCanvas() override;

	const std::shared_ptr<s2::Camera>& GetCamera() const { return m_cam2d; }

protected:
	virtual void OnSize(int w, int h) override;

protected:
	sm::rect GetVisibleRegion() const;
	float GetCameraScale() const;

private:
	void UpdateCam3D(int w, int h);

protected:
	std::shared_ptr<s2::Camera> m_cam2d = nullptr;

	std::shared_ptr<pt3::Camera> m_cam3d = nullptr;

}; // CameraCanvas

}

#endif // _EASYEDITOR_CAMERA_CANVAS_H_