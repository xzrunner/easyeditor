#include "ee3/StageCanvas.h"
#include "ee3/StagePage.h"

#include <ee/color_config.h>

#include <node3/PrimitiveDraw.h>
#include <node3/RenderCtxStack.h>
#include <node3/DrawNode.h>
#include <sprite2/RenderCtxStack.h>

namespace ee3
{

StageCanvas::StageCanvas(StagePage* stage, wxGLContext* glctx, bool has2d)
	: ee::OnePassCanvas(stage, stage->GetStageImpl(), glctx, USE_CONTEXT_STACK | HAS_2D * has2d | HAS_3D)
	, m_stage(stage)
	, m_has2d(has2d)
	, m_camera(sm::vec3(0, 2, -2), sm::vec3(0, 0, 0), sm::vec3(0, 1, 0))
{
	stage->GetSubjectMgr().RegisterObserver(ee0::MSG_SET_CANVAS_DIRTY, this);
}

void StageCanvas::OnNotify(ee0::MessageID msg, const ee0::VariantSet& variants)
{
	switch (msg)
	{
	case ee0::MSG_SET_CANVAS_DIRTY:
		SetDirty();
		break;
	}
}

//sm::vec2 StageCanvas::TransPos3ProjectToScreen(const sm::vec3& proj) const
//{
//	return m_viewport.TransPos3ProjectToScreen(m_mat_projection * GetCamera().GetModelViewMat() * proj, m_camera);
//}
//
//sm::vec3 StageCanvas::TransPos3ScreenToDir(const sm::vec2& screen) const
//{
//	return m_viewport.TransPos3ScreenToDir(screen, m_camera);
//}

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

	auto mt = m_camera.GetModelViewMat();
	for (auto& node : nodes) {
		n3::DrawNode::Draw(node, mt);
	}
}

}