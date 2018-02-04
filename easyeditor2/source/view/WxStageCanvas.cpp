#include "ee2/WxStageCanvas.h"
#include "ee2/WxStagePage.h"

#include <ee0/color_config.h>
#include <ee0/EditOP.h>

#include <sprite2/RenderCtxStack.h>
#include <sprite2/RVG.h>
#include <sprite2/OrthoCamera.h>
#include <node2/DrawNode.h>

namespace ee2
{

WxStageCanvas::WxStageCanvas(WxStagePage* stage, std::shared_ptr<wxGLContext> glctx)
	: ee0::WxStageCanvas(stage, stage->GetImpl(), glctx, USE_CONTEXT_STACK | HAS_2D)
	, m_stage(stage)
{
	m_cam = std::make_shared<s2::OrthoCamera>();

	stage->GetSubjectMgr().RegisterObserver(ee0::MSG_SET_CANVAS_DIRTY, this);
}

void WxStageCanvas::OnNotify(ee0::MessageID msg, const ee0::VariantSet& variants)
{
	switch (msg)
	{
	case ee0::MSG_SET_CANVAS_DIRTY:
		SetDirty();
		break;
	}
}

void WxStageCanvas::OnSize(int w, int h)
{
	auto ctx = const_cast<s2::RenderContext*>(s2::RenderCtxStack::Instance()->Top());
	if (ctx)
	{
		ctx->SetViewport(0, 0, w, h);
		ctx->SetScreen(w, h);
		ctx->SetProjection(w, h);
	}
}

void WxStageCanvas::OnDrawSprites() const
{
	DrawBackground();
	DrawNodes();

	m_stage->GetImpl().GetEditOP()->OnDraw();
}

void WxStageCanvas::DrawBackground() const
{
	const float EDGE = 100;
	s2::RVG::SetColor(ee0::LIGHT_GREY);
	s2::RVG::Cross(nullptr, sm::vec2(0, 0), EDGE, EDGE);
}

void WxStageCanvas::DrawNodes() const
{
	auto& nodes = m_stage->GetAllNodes();
	for (auto& node : nodes) {
		n2::DrawNode::Draw(node, sm::Matrix2D());
	}
}

}