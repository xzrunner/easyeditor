#include "CameraCanvas.h"

#include <sprite2/RenderCtxStack.h>
#include <gum/OrthoCamera.h>
#include <gum/Pseudo3DCamera.h>

namespace ee
{

CameraCanvas::CameraCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, gum::CameraType cam_type,
						   wxGLContext* glctx, bool use_context_stack)
	: TwoPassCanvas(stage_wnd, stage, glctx, use_context_stack)
	, m_camera(NULL)
{
	if (cam_type == gum::CAM_ORTHO2D) {
		m_camera = new gum::OrthoCamera();
	} else if (cam_type == gum::CAM_PSEUDO3D) {
		m_camera = new gum::Pseudo3DCamera();
	}
}

CameraCanvas::~CameraCanvas()
{
	delete m_camera;
}

sm::rect CameraCanvas::GetVisibleRegion() const
{
	if (!m_camera || m_camera->Type() == gum::CAM_PSEUDO3D) {
		return sm::rect();
	}
	const s2::RenderContext* ctx = s2::RenderCtxStack::Instance()->Top();
	if (!ctx) {
		return sm::rect();
	}

	gum::OrthoCamera* ortho_cam = static_cast<gum::OrthoCamera*>(m_camera);
	float s = ortho_cam->GetScale();
	sm::rect r(sm::vec2(0, 0), ctx->GetProjWidth() * s, ctx->GetProjHeight() * s);
	r.Translate(ortho_cam->GetPosition());
	return r;
}

float CameraCanvas::GetCameraScale() const
{
	if (m_camera->Type() == gum::CAM_PSEUDO3D) {
		return 1;
	}
	return static_cast<gum::OrthoCamera*>(m_camera)->GetScale();
}

}