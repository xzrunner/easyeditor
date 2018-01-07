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
	, m_cam_uvn(sm::vec3(0, 0, -2), sm::vec3(0, 0, 0), sm::vec3(0, 1, 0))
{
}

void StageCanvas::Refresh()
{
	auto ctx = n3::RenderCtxStack::Instance()->Top();
	if (ctx) {
		const_cast<n3::RenderContext*>(ctx)->SetModelView(GetCameraUVN().GetModelViewMat());
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

sm::vec2 StageCanvas::TransPos3ProjectToScreen(const sm::vec3& proj) const
{
	sm::mat4 mat_modelview = GetCameraUVN().GetModelViewMat();
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

//	m_cam_uvn.SetScreenSize(w, h);

	ctx->SetScreen(w, h);

	float hh = 1.0f * h / w;
	const float CAM_NEAR = 1;
	const float CAM_FAR = 3;
	//const float CAM_NEAR = 0.1f;
	//const float CAM_FAR = 100;
	auto mat_proj = sm::mat4::Perspective(-1, 1, -hh, hh, CAM_NEAR, CAM_FAR);
	ctx->SetProjection(mat_proj);
}

}