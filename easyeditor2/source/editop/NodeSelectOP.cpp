#include "ee2/NodeSelectOP.h"
#include "ee2/StagePage.h"
#include "ee2/StageCanvas.h"
#include "ee2/DrawSelectRectState.h"

#include <ee0/CameraHelper.h>

#include <node2/CompBoundingBox.h>
#include <sprite2/RVG.h>

namespace ee2
{

NodeSelectOP::NodeSelectOP(StagePage& stage)
	: ee0::NodeSelectOP(stage)
{
	auto cam = std::dynamic_pointer_cast<StageCanvas>(m_stage.GetCanvas())->GetCamera();
	GD_ASSERT(cam, "null cam");

	SetPrevEditOP(std::make_shared<CamControlOP>(
		&stage, stage.GetStageImpl(), *cam, stage.GetSubjectMgr()));

	m_draw_state = std::make_unique<DrawSelectRectState>(*cam, stage.GetSubjectMgr());
}

bool NodeSelectOP::OnMouseLeftDown(int x, int y)
{
	if (ee0::NodeSelectOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	m_draw_state->OnMousePress(x, y);

	return false;
}

bool NodeSelectOP::OnMouseLeftUp(int x, int y)
{
	if (ee0::NodeSelectOP::OnMouseLeftUp(x, y)) {
		return true;
	}

	m_draw_state->OnMouseRelease(x, y);

	return false;
}

bool NodeSelectOP::OnMouseDrag(int x, int y)
{
	if (ee0::NodeSelectOP::OnMouseDrag(x, y)) {
		return true;
	}

	m_draw_state->OnMouseDrag(x, y);

	return false;
}

bool NodeSelectOP::OnDraw() const
{
	if (ee0::NodeSelectOP::OnDraw()) {
		return true;
	}

	m_stage.GetNodeSelection().Traverse(
		[](const n0::SceneNodePtr& node)->bool
		{
			CU_VEC<sm::vec2> bound;
			auto& cbb = node->GetComponent<n2::CompBoundingBox>();
			cbb.GetBounding().GetBoundPos(bound);

			s2::RVG::SetColor(s2::Color(255, 0, 0));
			s2::RVG::Polyline(nullptr, bound, true);

			return true;
		}
	);

	m_draw_state->OnDraw();

	return false;
}

n0::SceneNodePtr NodeSelectOP::QueryByPos(int screen_x, int screen_y) const
{
	auto cam = std::dynamic_pointer_cast<StageCanvas>(m_stage.GetCanvas())->GetCamera();
	GD_ASSERT(cam, "null cam");
	auto pos = ee0::CameraHelper::TransPosScreenToProject(*cam, screen_x, screen_y);

	auto& nodes = dynamic_cast<StagePage&>(m_stage).GetAllNodes();
	for (auto& node : nodes)
	{
		auto& cbounding = node->GetComponent<n2::CompBoundingBox>();
		if (cbounding.GetBounding().IsContain(pos)) {
			return node;
		}
	}

	return nullptr;
}

void NodeSelectOP::QueryByRect(const sm::ivec2& p0, const sm::ivec2& p1, bool contain, 
	                           std::vector<n0::SceneNodePtr>& result) const
{
	auto cam = std::dynamic_pointer_cast<StageCanvas>(m_stage.GetCanvas())->GetCamera();
	GD_ASSERT(cam, "null cam");
	auto pos0 = ee0::CameraHelper::TransPosScreenToProject(*cam, p0.x, p0.y);
	auto pos1 = ee0::CameraHelper::TransPosScreenToProject(*cam, p1.x, p1.y);
	sm::rect rect(pos0, pos1);
	
	auto& nodes = dynamic_cast<StagePage&>(m_stage).GetAllNodes();
	for (auto& node : nodes)
	{
		auto& cbounding = node->GetComponent<n2::CompBoundingBox>();
		auto& bb = cbounding.GetBounding();
		if (contain && sm::is_rect_contain_rect(rect, bb.GetSize())) {
			result.push_back(node);
		} else if (!contain && bb.IsIntersect(rect)) {
			result.push_back(node);
		}
	}
}

}