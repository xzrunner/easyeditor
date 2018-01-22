#include "editop/NodeSelectOP.h"
#include "view/StagePanel.h"

#include <ee/FetchAllVisitor.h>
#include <ee/color_config.h>

#include <guard/check.h>
#include <node3/PrimitiveDraw.h>
#include <node3/Ray.h>
#include <node3/Math.h>

namespace eone
{

NodeSelectOP::NodeSelectOP(StagePanel& stage)
	: ee::EditOP(&stage, stage.GetStageImpl())
	, m_stage(stage)
{
}

bool NodeSelectOP::OnKeyDown(int keyCode)
{
	if (ee::EditOP::OnKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE)
	{
		VariantSet vars;
		Variant var;
		var.m_type = VT_PVOID;
		var.m_val.pv = const_cast<ee::SelectionSet<SceneNode>*>
			(&m_stage.GetNodeSelection());
		vars.SetVariant("selection", var);

		bool succ = m_stage.GetSubjectMgr().NotifyObservers(MSG_INSERT_SCENE_NODE, vars);
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
		VariantSet vars;
		Variant var;
		var.m_type = VT_PVOID;
		var.m_val.pv = const_cast<ee::SelectionSet<SceneNode>*>
			(&m_stage.GetNodeSelection());
		vars.SetVariant("node", var);

		if (m_stage.GetKeyState(WXK_CONTROL)) 
		{
			if (selection.IsExist(selected)) {
				sub_mgr.NotifyObservers(MSG_NODE_SELECTION_DELETE, vars);
			} else {
				sub_mgr.NotifyObservers(MSG_NODE_SELECTION_INSERT, vars);
			}
		}
		else
		{
			if (!selection.IsExist(selected))
			{
				sub_mgr.NotifyObservers(MSG_NODE_SELECTION_CLEAR);
				sub_mgr.NotifyObservers(MSG_NODE_SELECTION_INSERT, vars);
			}
		}
	}
	else
	{
		sub_mgr.NotifyObservers(MSG_NODE_SELECTION_CLEAR);
	}

	sub_mgr.NotifyObservers(MSG_SET_CANVAS_DIRTY);

	return false;
}

bool NodeSelectOP::OnDraw() const
{
	if (ee::EditOP::OnDraw()) return true;

	m_stage.GetNodeSelection().Traverse(
		[](const SceneNodePtr& node)->bool 
		{
			n3::PrimitiveDraw::SetColor(ee::MID_RED.ToABGR());

			auto& ctrans = node->GetComponent<n3::CompTransform>();
//			n3::PrimitiveDraw::Cube(ctrans.GetTransformMat(), node->GetModel()->GetAABB());

			return true;
		}
	);

	return false;
}

// AABB not changed, transform ray from Camera and spr's mat
SceneNodePtr NodeSelectOP::SelectByPos(const sm::vec2& pos) const
{
	//auto& nodes = m_stage.GetAllNodes();

	//StageCanvas* canvas = static_cast<StageCanvas*>(m_stage.GetCanvas());
	//sm::vec3 ray_dir = canvas->TransPos3ScreenToDir(pos);
	//n3::Ray ray(canvas->GetCamera().GetPos(), ray_dir);
	//for (auto& node : nodes)
	//{				
	//	sm::vec3 cross;
	//	bool intersect = n3::Math::RayOBBIntersection(
	//		node->GetModel()->GetAABB(), 
	//		node->GetPos(), 
	//		node->GetAngle(), 
	//		ray, 
	//		&cross);
	//	if (intersect) {
	//		return node;
	//	}
	//}

	return nullptr;
}

}