#include "view/SceneTreeCtrl.h"
#include "view/SceneTreeItem.h"
#include "msg/MessageID.h"
#include "msg/SubjectMgr.h"
#include "data/CompEditor.h"

#include <guard/check.h>

#include <queue>

namespace eone
{


BEGIN_EVENT_TABLE(SceneTreeCtrl, wxTreeCtrl)
	EVT_TREE_SEL_CHANGED(ID_SCENE_TREE_CTRL, SceneTreeCtrl::OnSelChanged)
	//EVT_TREE_SEL_CHANGING(ID_SCENE_TREE_CTRL, SceneTreeCtrl::OnSelChanging)
	EVT_TREE_END_LABEL_EDIT(ID_SCENE_TREE_CTRL, SceneTreeCtrl::OnLabelEdited)
END_EVENT_TABLE()

SceneTreeCtrl::SceneTreeCtrl(wxWindow* parent, SubjectMgr& sub_mgr)
	: wxTreeCtrl(parent, ID_SCENE_TREE_CTRL, wxDefaultPosition, wxDefaultSize,
		wxTR_HIDE_ROOT | wxTR_EDIT_LABELS | wxTR_MULTIPLE | wxTR_NO_LINES | wxTR_DEFAULT_STYLE)
	, m_sub_mgr(sub_mgr)
{
	SetBackgroundColour(wxColour(229, 229, 229));

	InitRoot();

	sub_mgr.RegisterObserver(MSG_INSERT_SCENE_NODE, this);
	sub_mgr.RegisterObserver(MSG_NODE_SELECTION_INSERT, this);
	sub_mgr.RegisterObserver(MSG_NODE_SELECTION_DELETE, this);
	sub_mgr.RegisterObserver(MSG_NODE_SELECTION_CLEAR, this);
}

void SceneTreeCtrl::OnNotify(MessageID msg, const VariantSet& variants)
{
	switch (msg)
	{
	case MSG_INSERT_SCENE_NODE:
		InsertSceneNode(variants);
		break;
	case MSG_NODE_SELECTION_INSERT:
		SelectSceneNode(variants);
		break;
	case MSG_NODE_SELECTION_DELETE:
		UnselectSceneNode(variants);
		break;
	case MSG_NODE_SELECTION_CLEAR:
		UnselectAll();
		break;
	}
}

void SceneTreeCtrl::Traverse(wxTreeItemId id, std::function<bool(wxTreeItemId)> func) const
{
	std::queue<wxTreeItemId> buf;
	buf.push(id);
	while (!buf.empty()) 
	{
		wxTreeItemId item = buf.front(); buf.pop();

		wxTreeItemIdValue cookie;
		wxTreeItemId id = GetFirstChild(item, cookie);
		if (id.IsOk()) 
		{
			if (item != id) 
			{
				// visit index
				bool stop = func(item);
				if (stop) {
					break;
				}
			}

			while (id.IsOk()) {
				buf.push(id);
				id = GetNextSibling(id);
			}
		} 
		else 
		{
			// visit leaf
			bool stop = func(item);
			if (stop) {
				break;
			}
		}
	}
}

void SceneTreeCtrl::InitRoot()
{
	m_root = AddRoot("ROOT");
	SetItemData(m_root, new SceneTreeItem());
}

void SceneTreeCtrl::OnSelChanged(wxTreeEvent& event)
{
	auto id = event.GetItem();
	if (!id.IsOk()) {
		return;
	}

	auto data = (SceneTreeItem*)GetItemData(id);
	auto& node = data->GetNode();
	GD_ASSERT(node, "err scene node.");

	VariantSet vars;

	Variant var;
	var.m_type = VT_PVOID;
	var.m_val.pv = &std::const_pointer_cast<n3::SceneNode>(node);
	vars.SetVariant("node", var);

	var.m_type = VT_BOOL;
	var.m_val.bl = true;
	vars.SetVariant("clear", var);

	m_sub_mgr.NotifyObservers(MSG_NODE_SELECTION_INSERT, vars);
}

//void SceneTreeCtrl::OnSelChanging(wxTreeEvent& event)
//{
//	auto id = event.GetItem();
//	if (!id.IsOk()) {
//		return;
//	}
//
//	auto data = (SceneTreeItem*)GetItemData(id);
//	auto& node = data->GetNode();
//	GD_ASSERT(node, "err scene node.");
//
//	VariantSet vars;
//	Variant var;
//	var.m_type = VT_PVOID;
//	var.m_val.pv = &std::const_pointer_cast<SceneNode>(node);
//	vars.SetVariant("node", var);
//
//	m_sub_mgr.NotifyObservers(MSG_NODE_SELECTION_DELETE, vars);
//}

void SceneTreeCtrl::OnLabelEdited(wxTreeEvent& event)
{

}

void SceneTreeCtrl::SelectSceneNode(const VariantSet& variants)
{
	auto var = variants.GetVariant("node");
	GD_ASSERT(var.m_type != VT_EMPTY, "no var in vars: node");
	n3::SceneNodePtr* node = static_cast<n3::SceneNodePtr*>(var.m_val.pv);
	GD_ASSERT(node, "err scene node");

	Traverse(m_root, [&](wxTreeItemId item)->bool
		{
			auto pdata = (SceneTreeItem*)GetItemData(item);
			if (pdata->GetNode() == *node) {
				SelectItem(item);
				return true;
			} else {
				return false;
			}
		}
	);
}

void SceneTreeCtrl::UnselectSceneNode(const VariantSet& variants)
{
	auto var = variants.GetVariant("node");
	GD_ASSERT(var.m_type != VT_EMPTY, "no var in vars: node");
	n3::SceneNodePtr* node = static_cast<n3::SceneNodePtr*>(var.m_val.pv);
	GD_ASSERT(node, "err scene node");

	Traverse(m_root, [&](wxTreeItemId item)->bool
		{
			auto pdata = (SceneTreeItem*)GetItemData(item);
			if (pdata->GetNode() == *node) {
				UnselectItem(item);
				return true;
			} else {
				return false;
			}
		}
	);
}

void SceneTreeCtrl::InsertSceneNode(const VariantSet& variants)
{
	auto var = variants.GetVariant("node");
	GD_ASSERT(var.m_type != VT_EMPTY, "no var in vars: node");
	n3::SceneNodePtr* node = static_cast<n3::SceneNodePtr*>(var.m_val.pv);
	GD_ASSERT(node, "err scene node");

	wxTreeItemId parent = GetFocusedItem();
	if (!parent.IsOk()) {
		parent = m_root;
	}

	InsertSceneNode(parent, *node);

	if (parent != m_root) 
	{
		auto pdata = (SceneTreeItem*)GetItemData(parent);
		pdata->GetNode()->AddChild(*node);
		(*node)->SetParent(pdata->GetNode());
		Expand(parent);
	}
}

void SceneTreeCtrl::InsertSceneNode(wxTreeItemId parent, const n3::SceneNodePtr& node)
{
	auto item = new SceneTreeItem(node);

	auto pdata = (SceneTreeItem*)GetItemData(parent);
	auto pos = pdata->GetChildrenNum();
	pdata->AddChild(item);

	auto& ceditor = node->GetComponent<CompEditor>();
	wxTreeItemId id = InsertItem(parent, pos, ceditor.GetName());
	SetItemData(id, item);

	for (auto& child : node->GetAllChildren()) {
		InsertSceneNode(id, std::dynamic_pointer_cast<n3::SceneNode>(child));
	}
}

}