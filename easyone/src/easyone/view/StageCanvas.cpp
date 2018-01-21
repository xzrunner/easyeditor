#include "view/StageCanvas.h"
#include "view/StagePanel.h"

#include <ee/color_config.h>
#include <ee/panel_msg.h>

#include <node3/PrimitiveDraw.h>
#include <node3/RenderCtxStack.h>
#include <node3/CompModel.h>
#include <node3/RenderSystem.h>

namespace eone
{

StageCanvas::StageCanvas(StagePanel* stage, wxGLContext* glctx)
	: ee::OnePassCanvas(stage, stage->GetStageImpl(), glctx, USE_CONTEXT_STACK | HAS_3D)
	, m_stage(stage)
	, m_camera(sm::vec3(0, 2, -2), sm::vec3(0, 0, 0), sm::vec3(0, 1, 0))
{
}

void StageCanvas::RefreshCanvas()
{
	auto ctx = n3::RenderCtxStack::Instance()->Top();
	if (ctx) {
		const_cast<n3::RenderContext*>(ctx)->SetModelView(GetCamera().GetModelViewMat());
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

sm::vec2 StageCanvas::TransPos3ProjectToScreen(const sm::vec3& proj) const
{
	return m_viewport.TransPos3ProjectToScreen(m_mat_projection * GetCamera().GetModelViewMat() * proj, m_camera);
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
}

void StageCanvas::OnDrawSprites() const
{
	auto ctx = const_cast<n3::RenderContext*>(n3::RenderCtxStack::Instance()->Top());
	if (!ctx) {
		return;
	}
	const_cast<n3::RenderContext*>(ctx)->SetModelView(GetCamera().GetModelViewMat());

	DrawBackground();
	DrawNodes();

	m_stage->DrawEditOP();
}

void StageCanvas::DrawBackground() const
{
	n3::PrimitiveDraw::SetColor(0xff000088);
	n3::PrimitiveDraw::Cross(sm::vec3(0, 0, 0), sm::vec3(100, 100, 100));

	static const int TOT_LEN = 100;
	static const int GRID_EDGE = 5;
	for (int z = -TOT_LEN; z < TOT_LEN; z += GRID_EDGE) {
		for (int x = -TOT_LEN; x < TOT_LEN; x += GRID_EDGE) {
			if ((x + z) % (GRID_EDGE * 2) == 0) {
				n3::PrimitiveDraw::SetColor(0xff444444);
			} else {
				n3::PrimitiveDraw::SetColor(0xff888888);
			}
			n3::PrimitiveDraw::Rect(
				sm::vec3(x, 0, z),
				sm::vec3(x + GRID_EDGE, 0, z + GRID_EDGE));
		}
	}
}

void StageCanvas::DrawNodes() const
{
	auto& nodes = m_stage->GetAllNodes();

	// draw model
	for (auto& node : nodes)
	{
		if (!node->HasComponent<n3::CompModel>()) {
			continue;
		}

		auto& cmodel = node->GetComponent<n3::CompModel>();
		assert(node->HasComponent<n3::CompTransform>());
		auto& ctrans = node->GetComponent<n3::CompTransform>();
		
		n3::RenderSystem::DrawModel(cmodel.GetModel(), ctrans.GetTransformMat());
	}
}

}