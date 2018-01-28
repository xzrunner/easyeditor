#include "ee3/NodeSelectOP.h"
#include "ee3/StagePage.h"
#include "ee3/StageCanvas.h"

#include <ee/FetchAllVisitor.h>
#include <ee/color_config.h>

#include <ee0/MessageID.h>

#include <guard/check.h>
#include <node3/PrimitiveDraw.h>
#include <node3/Ray.h>
#include <node3/Math.h>
#include <node3/CompAABB.h>
#include <node3/CompTransform.h>

namespace ee3
{

NodeSelectOP::NodeSelectOP(StagePage& stage)
	: ee::EditOP(&stage, stage.GetStageImpl())
	, m_stage(stage)
{
}

bool NodeSelectOP::OnKeyDown(int keyCode)
{
	if (ee::EditOP::OnKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE)
	{
		ee0::VariantSet vars;
		ee0::Variant var;
		var.m_type = ee0::VT_PVOID;
		var.m_val.pv = const_cast<ee::SelectionSet<n0::SceneNode>*>
			(&m_stage.GetNodeSelection());
		vars.SetVariant("selection", var);

		bool succ = m_stage.GetSubjectMgr().NotifyObservers(ee0::MSG_NODE_SELECTION_DELETE, vars);
		GD_ASSERT(succ, "no MSG_INSERT_SCENE_NODE");
	}

	return false;
}

bool NodeSelectOP::OnMouseLeftDown(int x, int y)
{
	if (ee::EditOP::OnMouseLeftDown(x, y)) return true;

	auto& sub_mgr = m_stage.GetSubjectMgr();

	auto& selection = m_stage.GetNodeSelection();
	auto selected = SelectByPos(sm::vec2(x, y));
	if (selected)
	{
		ee0::VariantSet vars;
		ee0::Variant var;
		var.m_type = ee0::VT_PVOID;
		var.m_val.pv = &selected;
		vars.SetVariant("node", var);

		if (m_stage.GetKeyState(WXK_CONTROL)) 
		{
			if (selection.IsExist(selected)) {
				sub_mgr.NotifyObservers(ee0::MSG_NODE_SELECTION_DELETE, vars);
			} else {
				sub_mgr.NotifyObservers(ee0::MSG_NODE_SELECTION_INSERT, vars);
			}
		}
		else
		{
			if (!selection.IsExist(selected))
			{
				sub_mgr.NotifyObservers(ee0::MSG_NODE_SELECTION_CLEAR);
				sub_mgr.NotifyObservers(ee0::MSG_NODE_SELECTION_INSERT, vars);
			}
		}
	}
	else
	{
		sub_mgr.NotifyObservers(ee0::MSG_NODE_SELECTION_CLEAR);
	}

	sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);

	return false;
}

bool NodeSelectOP::OnDraw() const
{
	if (ee::EditOP::OnDraw()) return true;

	m_stage.GetNodeSelection().Traverse(
		[](const n0::SceneNodePtr& node)->bool
		{
			n3::PrimitiveDraw::SetColor(ee::MID_RED.ToABGR());

			auto& caabb = node->GetComponent<n3::CompAABB>();
			auto& ctrans = node->GetComponent<n3::CompTransform>();

			sm::mat4 prev_mt;
			auto parent = node->GetParent();
			while (parent)
			{
				auto& pctrans = parent->GetComponent<n3::CompTransform>();
				prev_mt = pctrans.GetTransformMat() * prev_mt;
				parent = parent->GetParent();
			}

			n3::PrimitiveDraw::Cube(prev_mt * ctrans.GetTransformMat(), caabb.GetAABB());

			return true;
		}
	);

	return false;
}

// AABB not changed, transform ray from Camera and spr's mat
n0::SceneNodePtr NodeSelectOP::SelectByPos(const sm::vec2& pos) const
{
	auto& nodes = m_stage.GetAllNodes();

	auto canvas = std::dynamic_pointer_cast<StageCanvas>(m_stage.GetCanvas());
	auto& vp = canvas->GetViewport();
	auto& cam = canvas->GetCamera();
	sm::vec3 ray_dir = vp.TransPos3ScreenToDir(pos, cam);
	n3::Ray ray(cam.GetPos(), ray_dir);
	for (auto& node : nodes)
	{
		auto& caabb = node->GetComponent<n3::CompAABB>();
		auto& ctrans = node->GetComponent<n3::CompTransform>();

		sm::vec3 cross;
		bool intersect = n3::Math::RayOBBIntersection(
			caabb.GetAABB(),
			ctrans.GetPosition(), 
			ctrans.GetAngle(), 
			ray, 
			&cross);
		if (intersect) {
			return node;
		}
	}

	return nullptr;
}

}