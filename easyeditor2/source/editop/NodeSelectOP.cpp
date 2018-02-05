#include "ee2/NodeSelectOP.h"
#include "ee2/WxStagePage.h"
#include "ee2/WxStageCanvas.h"
#include "ee2/DrawSelectRectState.h"

#include <ee0/CameraHelper.h>

#include <node0/SceneNode.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>
#include <sprite2/RVG.h>
#include <guard/check.h>
#include <painting2/Color.h>

namespace ee2
{

NodeSelectOP::NodeSelectOP(WxStagePage& stage)
	: ee0::NodeSelectOP(stage)
	, m_draw_state_disable(false)
{
	auto cam = std::dynamic_pointer_cast<WxStageCanvas>(m_stage.GetImpl().GetCanvas())->GetCamera();
	GD_ASSERT(cam, "null cam");

	SetPrevEditOP(std::make_shared<CamControlOP>(*cam, stage.GetSubjectMgr()));

	m_draw_state = std::make_unique<DrawSelectRectState>(*cam, stage.GetSubjectMgr());
}

bool NodeSelectOP::OnMouseLeftDown(int x, int y)
{
	if (ee0::NodeSelectOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	if (!m_draw_state_disable) {
		m_draw_state->OnMousePress(x, y);
	}

	return false;
}

bool NodeSelectOP::OnMouseLeftUp(int x, int y)
{
	if (ee0::NodeSelectOP::OnMouseLeftUp(x, y)) {
		return true;
	}

	if (!m_draw_state_disable) {
		m_draw_state->OnMouseRelease(x, y);
	}

	m_draw_state_disable = false;
	m_draw_state->Clear();

	return false;
}

bool NodeSelectOP::OnMouseDrag(int x, int y)
{
	if (ee0::NodeSelectOP::OnMouseDrag(x, y)) {
		return true;
	}

	if (!m_draw_state_disable) {
		m_draw_state->OnMouseDrag(x, y);
	}

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

			// todo
			sm::Matrix2D world_mt;
			auto parent = node->GetParent();
			while (parent) {
				auto& ctrans = parent->GetComponent<n2::CompTransform>();
				world_mt = ctrans.GetTrans().GetMatrix() * world_mt;
				parent = parent->GetParent();
			}
			for (auto& pos : bound) {
				pos = world_mt * pos;
			}

			s2::RVG::SetColor(pt2::Color(255, 0, 0));
			s2::RVG::Polyline(nullptr, bound, true);

			return true;
		}
	);

	if (!m_draw_state_disable) {
		m_draw_state->OnDraw();
	}

	return false;
}

n0::SceneNodePtr NodeSelectOP::QueryByPos(int screen_x, int screen_y) const
{
	auto cam = std::dynamic_pointer_cast<WxStageCanvas>(m_stage.GetImpl().GetCanvas())->GetCamera();
	GD_ASSERT(cam, "null cam");
	auto pos = ee0::CameraHelper::TransPosScreenToProject(*cam, screen_x, screen_y);

	auto& nodes = dynamic_cast<WxStagePage&>(m_stage).GetAllNodes();
	for (auto& node : nodes)
	{
		auto ret = QueryByPos(node, pos);
		if (ret) {
			m_draw_state_disable = true;
			return ret;
		}
	}

	return nullptr;
}

void NodeSelectOP::QueryByRect(const sm::ivec2& p0, const sm::ivec2& p1, bool contain, 
	                           std::vector<n0::SceneNodePtr>& result) const
{
	auto cam = std::dynamic_pointer_cast<WxStageCanvas>(m_stage.GetImpl().GetCanvas())->GetCamera();
	GD_ASSERT(cam, "null cam");
	auto pos0 = ee0::CameraHelper::TransPosScreenToProject(*cam, p0.x, p0.y);
	auto pos1 = ee0::CameraHelper::TransPosScreenToProject(*cam, p1.x, p1.y);
	sm::rect rect(pos0, pos1);
	
	auto& nodes = dynamic_cast<WxStagePage&>(m_stage).GetAllNodes();
	for (auto& node : nodes) {
		QueryByRect(node, rect, contain, result);
	}
}

n0::SceneNodePtr NodeSelectOP::QueryByPos(const n0::SceneNodePtr& node, const sm::vec2& pos) const
{
	auto& cbounding = node->GetComponent<n2::CompBoundingBox>();
	if (cbounding.GetBounding().IsContain(pos)) {
		return node;
	}

	auto& children = node->GetAllChildren();
	auto mt = node->GetComponent<n2::CompTransform>().GetTrans().GetMatrix().Inverted();
	sm::vec2 child_pos = mt * pos;
	for (auto& child : children) {
		auto ret = QueryByPos(child, child_pos);
		if (ret) {
			return ret;
		}
	}

	return nullptr;
}

void NodeSelectOP::QueryByRect(const n0::SceneNodePtr& node, const sm::rect& rect, 
	                           bool contain, std::vector<n0::SceneNodePtr>& result) const
{
	auto& cbounding = node->GetComponent<n2::CompBoundingBox>();
	auto& bb = cbounding.GetBounding();
	if (contain && sm::is_rect_contain_rect(rect, bb.GetSize())) {
		result.push_back(node);
	} else if (!contain && bb.IsIntersect(rect)) {
		result.push_back(node);
	}

	auto& children = node->GetAllChildren();
	// todo
//	auto& mt = node->GetComponent<n2::CompTransform>().GetTransformMat();
	for (auto& child : children) {
		QueryByRect(child, rect, contain, result);
	}

	m_draw_state_disable = !result.empty();
}

}