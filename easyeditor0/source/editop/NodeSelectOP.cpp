#include "ee0/NodeSelectOP.h"
#include "ee0/StagePage.h"

#include <ee/FetchAllVisitor.h>
#include <ee/color_config.h>

#include <ee0/MessageID.h>

#include <guard/check.h>

namespace ee0
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
	auto selected = SelectByPos(x, y);
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

}