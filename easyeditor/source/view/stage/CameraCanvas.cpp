#include "CameraCanvas.h"

#include <sprite2/RenderCtxStack.h>
#include <sprite2/OrthoCamera.h>
#include <sprite2/Pseudo3DCamera.h>

namespace ee
{

CameraCanvas::CameraCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, s2::CameraType cam_type,
						   wxGLContext* glctx, bool use_context_stack)
	: TwoPassCanvas(stage_wnd, stage, glctx, use_context_stack)
	, m_camera(NULL)
{
	if (cam_type == s2::CAM_ORTHO2D) {
		m_camera = new s2::OrthoCamera();
	} else if (cam_type == s2::CAM_PSEUDO3D) {
		m_camera = new s2::Pseudo3DCamera();
	}
}

CameraCanvas::~CameraCanvas()
{
	delete m_camera;
}

sm::rect CameraCanvas::GetVisibleRegion() const
{
	if (!m_camera || m_camera->Type() == s2::CAM_PSEUDO3D) {
		return sm::rect();
	}
	const s2::RenderContext* ctx = s2::RenderCtxStack::Instance()->Top();
	if (!ctx) {
		return sm::rect();
	}

	s2::OrthoCamera* ortho_cam = static_cast<s2::OrthoCamera*>(m_camera);
	float s = ortho_cam->GetScale();
	sm::rect r(sm::vec2(0, 0), ctx->GetProjWidth() * s, ctx->GetProjHeight() * s);
	r.Translate(ortho_cam->GetPosition());
	return r;
}

float CameraCanvas::GetCameraScale() const
{
	if (m_camera->Type() == s2::CAM_PSEUDO3D) {
		return 1;
	}
	return static_cast<s2::OrthoCamera*>(m_camera)->GetScale();
}

}