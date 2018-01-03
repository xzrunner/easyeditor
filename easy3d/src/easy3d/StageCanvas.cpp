#include "StageCanvas.h"
#include "ViewFrustum.h"

#include <ee/panel_msg.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>

#include <node3/RenderCtxStack.h>
#include <gum/RenderContext.h>

namespace e3d
{

StageCanvas::StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage)
	: ee::OnePassCanvas(stage_wnd, stage, nullptr, true, true)
{
}

void StageCanvas::Refresh()
{
	auto ctx = n3::RenderCtxStack::Instance()->Top();
	if (ctx) {
		const_cast<n3::RenderContext*>(ctx)->SetModelView(GetCamera3().GetModelViewMat());
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

sm::ivec2 StageCanvas::TransPos3ProjectToScreen(const sm::vec3& proj) const
{
	sm::mat4 mat_modelview = GetCamera3().GetModelViewMat();
	sm::vec3 v0 = mat_modelview * proj;

	sm::vec3 v1 = m_mat_projection * v0;
	v1.z = v0.z;

	return ViewFrustum::TransPos3ProjectToScreen(v1, m_width, m_height);
}

void StageCanvas::OnSize(int w, int h)
{
	auto ctx = const_cast<n3::RenderContext*>(n3::RenderCtxStack::Instance()->Top());
	if (!ctx) {
		return;
	}

	m_camera3.SetScreenSize(w, h);

	ctx->SetScreen(w, h);

	float hh = 1.0f * h / w;
	auto mat_proj = sm::mat4::Perspective(-1, 1, -hh, hh, 
		e3d::Camera::CAM_NEAR, e3d::Camera::CAM_FAR);
	ctx->SetProjection(mat_proj);
}

}