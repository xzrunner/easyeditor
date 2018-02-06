#include "CameraCanvas.h"

#include <sprite2/RenderCtxStack.h>
#include <sprite2/OrthoCamera.h>
#include <sprite2/Pseudo3DCamera.h>
#include <sprite2/Blackboard.h>
#include <node3/RenderCtxStack.h>
#include <painting3/Camera.h>

namespace ee
{

CameraCanvas::CameraCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, s2::CameraType cam_type,
						   wxGLContext* glctx, bool use_context_stack, bool has_3d)
	: TwoPassCanvas(stage_wnd, stage, glctx, USE_CONTEXT_STACK * use_context_stack | HAS_2D | HAS_3D * has_3d)
	, m_cam2d(nullptr)
	, m_cam3d(nullptr)
{
	if (cam_type == s2::CAM_ORTHO2D) {
		m_cam2d = std::make_shared<s2::OrthoCamera>();
	} else if (cam_type == s2::CAM_PSEUDO3D) {
		m_cam2d = std::make_shared<s2::Pseudo3DCamera>();
	}

	if (has_3d) {
		m_cam3d = std::make_shared<pt3::Camera>(sm::vec3(0, 0, 2), sm::vec3(0, 0, 0), sm::vec3(0, 1, 0));
	}
}

CameraCanvas::~CameraCanvas()
{
	s2::Blackboard::Instance()->SetCamera(NULL);
}

void CameraCanvas::SetCurrentCanvas()
{
	TwoPassCanvas::SetCurrentCanvas();
	s2::Blackboard::Instance()->SetCamera(m_cam2d);
}

void CameraCanvas::OnSize(int w, int h)
{
	TwoPassCanvas::OnSize(w, h);

	UpdateCam3D(w, h);
}

sm::rect CameraCanvas::GetVisibleRegion() const
{
	if (!m_cam2d || m_cam2d->Type() == s2::CAM_PSEUDO3D) {
		return sm::rect();
	}
	const s2::RenderContext* ctx = s2::RenderCtxStack::Instance()->Top();
	if (!ctx) {
		return sm::rect();
	}

	auto ortho_cam = std::dynamic_pointer_cast<s2::OrthoCamera>(m_cam2d);
	float s = ortho_cam->GetScale();
	sm::rect r(sm::vec2(0, 0), ctx->GetScreenWidth() * s, ctx->GetScreenHeight() * s);
	r.Translate(ortho_cam->GetPosition());
	return r;
}

float CameraCanvas::GetCameraScale() const
{
	if (m_cam2d->Type() == s2::CAM_PSEUDO3D) {
		return 1;
	}
	return std::dynamic_pointer_cast<s2::OrthoCamera>(m_cam2d)->GetScale();
}

void CameraCanvas::UpdateCam3D(int w, int h)
{
	if (!m_cam3d) {
		return;
	}

	auto ctx = const_cast<n3::RenderContext*>(n3::RenderCtxStack::Instance()->Top());
	if (!ctx) {
		return;
	}

	ctx->SetModelView(m_cam3d->GetModelViewMat());

	m_cam3d->SetAspect((float)w / h);
	ctx->SetProjection(m_cam3d->GetProjectionMat());
}

}