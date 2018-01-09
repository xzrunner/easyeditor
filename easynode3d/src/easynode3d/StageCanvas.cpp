#include "StageCanvas.h"

#include <ee/panel_msg.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>

#include <node3/RenderCtxStack.h>
#include <node3/Camera.h>
#include <node3/Viewport.h>
#include <gum/RenderContext.h>

namespace enode3d
{

StageCanvas::StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage)
	: ee::OnePassCanvas(stage_wnd, stage, nullptr, true, true)
	, m_camera(sm::vec3(0, 0, -2), sm::vec3(0, 0, 0), sm::vec3(0, 1, 0))
{
}

void StageCanvas::Refresh()
{
	auto ctx = n3::RenderCtxStack::Instance()->Top();
	if (ctx) {
		const_cast<n3::RenderContext*>(ctx)->SetModelView(GetCamera().GetModelViewMat());
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

sm::vec2 StageCanvas::TransPos3ProjectToScreen(const sm::vec3& proj) const
{
	sm::mat4 mat_modelview = GetCamera().GetModelViewMat();
	sm::vec3 v0 = mat_modelview * proj;

	sm::vec3 v1 = m_mat_projection * v0;
	v1.z = v0.z;

	return m_viewport.TransPos3ProjectToScreen(v1, m_camera.GetNear(), m_camera.GetFar());
}

sm::vec3 StageCanvas::TransPos3ScreenToProject(const sm::vec2& scr, float proj_z) const
{
	sm::vec3 pos = m_camera.GetModelViewMat() * sm::vec3(0, 0, proj_z);
	return m_viewport.TransPos3ScreenToProject(scr, pos.z, m_camera.GetNear(), m_camera.GetFar());
}

sm::vec3 StageCanvas::TransPos3ScreenToDir(const sm::vec2& screen) const
{
	return m_viewport.TransPos3ScreenToDir(screen, m_camera.GetNear());
}

void StageCanvas::OnSize(int w, int h)
{
	auto ctx = const_cast<n3::RenderContext*>(n3::RenderCtxStack::Instance()->Top());
	if (!ctx) {
		return;
	}

	ctx->SetScreen(w, h);
	m_viewport.SetSize(w, h);

	m_camera.SetAspect((float)w / h);
	ctx->SetProjection(m_camera.GetProjectionMat());
}

}