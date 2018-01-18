#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/panel_msg.h>
#include <ee/EditPanelImpl.h>
#include <ee/color_config.h>

#include <node3/RenderCtxStack.h>
#include <node3/Camera.h>
#include <node3/Viewport.h>
#include <node3/PrimitiveDraw.h>
#include <node3/RenderParams.h>
#include <node3/DrawNode.h>
#include <sprite2/RenderCtxStack.h>
#include <unirender/RenderContext.h>

namespace enode3d
{

StageCanvas::StageCanvas(StagePanel* stage, wxGLContext* glctx, bool has2d)
	: ee::OnePassCanvas(stage, stage->GetStageImpl(), glctx, USE_CONTEXT_STACK | HAS_2D * has2d | HAS_3D)
	, m_stage(stage)
	, m_has2d(has2d)
	, m_camera(sm::vec3(0, 2, -2), sm::vec3(0, 0, 0), sm::vec3(0, 1, 0))
{
}

void StageCanvas::Refresh()
{
	auto ctx = n3::RenderCtxStack::Instance()->Top();
	if (ctx) {
		const_cast<n3::RenderContext*>(ctx)->SetModelView(GetCamera().GetModelViewMat());
	}

	if (m_has2d) {
		auto ctx = s2::RenderCtxStack::Instance()->Top();
		if (ctx) {
			const_cast<s2::RenderContext*>(ctx)->SetModelView(sm::vec2(0, 0), 1);
		}
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

sm::vec2 StageCanvas::TransPos3ProjectToScreen(const sm::vec3& proj) const
{
	sm::mat4 mat_modelview = GetCamera().GetModelViewMat();
	sm::vec3 v0 = mat_modelview * proj;

	sm::vec3 v1 = m_mat_projection * v0;
	v1.z = v0.z;

	return m_viewport.TransPos3ProjectToScreen(v1, m_camera);
}

sm::vec3 StageCanvas::TransPos3ScreenToDir(const sm::vec2& screen) const
{
	return m_viewport.TransPos3ScreenToDir(screen, m_camera);
}

void StageCanvas::OnSize(int w, int h)
{
	auto ctx = const_cast<n3::RenderContext*>(n3::RenderCtxStack::Instance()->Top());
	if (ctx)
	{
		ctx->SetScreen(w, h);
		m_viewport.SetSize(w, h);

		m_camera.SetAspect((float)w / h);
		ctx->SetProjection(m_camera.GetProjectionMat());
	}

	if (m_has2d)
	{
		auto ctx = const_cast<s2::RenderContext*>(s2::RenderCtxStack::Instance()->Top());
		if (ctx)
		{
			ctx->SetScreen(w, h);
			ctx->SetProjection(w, h);
		}
	}
}

void StageCanvas::OnDrawSprites() const
{
	auto ctx = const_cast<n3::RenderContext*>(n3::RenderCtxStack::Instance()->Top());
	if (!ctx) {
		return;
	}
	const_cast<n3::RenderContext*>(ctx)->SetModelView(GetCamera().GetModelViewMat());

	if (m_has2d)
	{
		auto ctx = const_cast<s2::RenderContext*>(s2::RenderCtxStack::Instance()->Top());
		if (ctx) {
			ctx->SetModelView(sm::vec2(0, 0), 1);
		}
	}
	
	DrawBackground();
	DrawSprites();

	m_stage->DrawEditOP();
}

void StageCanvas::DrawSprites() const
{
	auto& nodes = m_stage->GetAllNodes();
	n3::RenderParams rp(m_camera.GetModelViewMat());
	for (auto& node : nodes) {
		n3::DrawNode::Draw(node, rp);
	}
}

void StageCanvas::DrawBackground() const
{
	n3::PrimitiveDraw::SetColor(ee::LIGHT_RED.ToABGR());
	n3::PrimitiveDraw::Cross(sm::vec3(0, 0, 0), sm::vec3(10, 10, 10));
}

}