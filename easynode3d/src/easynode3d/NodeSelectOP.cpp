#include "NodeSelectOP.h"
#include "StagePanel.h"
#include "StageCanvas.h"

#include <ee/panel_msg.h>
#include <ee/FetchAllVisitor.h>
#include <ee/color_config.h>

#include <node3/PrimitiveDraw.h>
#include <node3/Ray.h>
#include <node3/Math.h>
#include <sprite2/ModelSprite.h>
#include <sprite2/ModelSymbol.h>
#include <sprite2/SymType.h>

namespace enode3d
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
		std::vector<n3::NodePtr> nodes;
		auto& selection = m_stage.GetNodeSelection();
		selection.Traverse(ee::FetchAllRefVisitor<n3::INode>(nodes));
		for (auto& node : nodes) {
			m_stage.DeleteNode(node);
		}
	}

	return false;
}

bool NodeSelectOP::OnMouseLeftDown(int x, int y)
{
	if (ee::EditOP::OnMouseLeftDown(x, y)) return true;

	auto& selection = m_stage.GetNodeSelection();
	auto selected = SelectByPos(sm::vec2(x, y));
	if (selected)
	{
		if (m_stage.GetKeyState(WXK_CONTROL)) 
		{
			if (selection.IsExist(selected)) {
				selection.Remove(selected);
			} else {
				selection.Add(selected);
			}
		}
		else
		{
			if (!selection.IsExist(selected))
			{
				selection.Clear();
				selection.Add(selected);
			}
		}
	}
	else
	{
		selection.Clear();
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool NodeSelectOP::OnDraw() const
{
	if (ee::EditOP::OnDraw()) return true;

	std::vector<n3::NodePtr> nodes;
	auto& selection = m_stage.GetNodeSelection();
	selection.Traverse(ee::FetchAllRefVisitor<n3::INode>(nodes));
	for (auto& node : nodes) 
	{
		auto& pos = node->GetPos();
		sm::mat4 mat = sm::mat4(node->GetAngle()) *
			sm::mat4::Translated(pos.x, pos.y, pos.z);
		n3::PrimitiveDraw::SetColor(ee::MID_RED.ToABGR());
		n3::PrimitiveDraw::Cube(mat, node->GetModel()->GetAABB());
	}

	return false;
}

// AABB not changed, transform ray from Camera and spr's mat
n3::NodePtr NodeSelectOP::SelectByPos(const sm::vec2& pos) const
{
	auto& nodes = m_stage.GetAllNodes();

	StageCanvas* canvas = static_cast<StageCanvas*>(m_stage.GetCanvas());
	sm::vec3 ray_dir = canvas->TransPos3ScreenToDir(pos);
	n3::Ray ray(canvas->GetCamera().GetPos(), ray_dir);
	for (auto& node : nodes)
	{				
		sm::vec3 cross;
		bool intersect = n3::Math::RayOBBIntersection(
			node->GetModel()->GetAABB(), 
			node->GetPos(), 
			node->GetAngle(), 
			ray, 
			&cross);
		if (intersect) {
			return node;
		}
	}

	return nullptr;
}

}