#include "GroupTreeImpl.h"
#include "GroupTreeItem.h"
#include "GroupTreeCtrl.h"

#include "dataset/Group.h"

namespace d2d
{
	
//////////////////////////////////////////////////////////////////////////
// class RemoveVisitor
//////////////////////////////////////////////////////////////////////////

GroupTreeImpl::RemoveVisitor::
RemoveVisitor(wxTreeCtrl* treectrl, ISprite* spr)
	: m_treectrl(treectrl)
	, m_spr(spr)
{
	if (m_spr) {
		m_spr->Retain();
	}
}

GroupTreeImpl::RemoveVisitor::
~RemoveVisitor()
{
	if (m_spr) {
		m_spr->Release();
	}
}

void GroupTreeImpl::RemoveVisitor::
VisitNonleaf(wxTreeItemId id)
{
	assert(id.IsOk());

	GroupTreeItem* data = (GroupTreeItem*)m_treectrl->GetItemData(id);
	if (!data) {
		return;
	}

	if (data->m_group) {
		data->m_group->Remove(m_spr);
	}
}

void GroupTreeImpl::RemoveVisitor::
VisitLeaf(wxTreeItemId id)
{
	assert(id.IsOk());
	GroupTreeItem* data = (GroupTreeItem*)m_treectrl->GetItemData(id);
	if (!data) {
		return;
	}

	if (data->m_sprite && data->m_sprite == m_spr) {
		delete data;
		m_treectrl->Delete(id);
	}
}

//////////////////////////////////////////////////////////////////////////
// class SelectVisitor
//////////////////////////////////////////////////////////////////////////

GroupTreeImpl::SelectVisitor::
SelectVisitor(wxTreeCtrl* treectrl, SpriteSelection* selection)
	: m_treectrl(treectrl)
	, m_selection(selection)
{
	if (m_selection) {
		m_selection->Retain();
	}
}

GroupTreeImpl::SelectVisitor::
~SelectVisitor()
{
	if (m_selection) {
		m_selection->Release();
	}
}

void GroupTreeImpl::SelectVisitor::
VisitLeaf(wxTreeItemId id)
{
	assert(id.IsOk());
	GroupTreeItem* data = (GroupTreeItem*)m_treectrl->GetItemData(id);
	if (!data) {
		return;
	}

	if (data->m_sprite) {
		m_selection->Add(data->m_sprite);
	}
}

//////////////////////////////////////////////////////////////////////////
// class VisibleVisitor
//////////////////////////////////////////////////////////////////////////

void GroupTreeImpl::VisibleVisitor::
VisitLeaf(wxTreeItemId id)
{
	assert(id.IsOk());
	GroupTreeItem* data = (GroupTreeItem*)m_treectrl->GetItemData(id);
	if (!data) {
		return;
	}

	if (data->m_sprite) {
		data->m_sprite->visiable = !data->m_sprite->visiable;
	}
}

//////////////////////////////////////////////////////////////////////////
// class EditableVisitor
//////////////////////////////////////////////////////////////////////////

void GroupTreeImpl::EditableVisitor::
VisitLeaf(wxTreeItemId id)
{
	assert(id.IsOk());
	GroupTreeItem* data = (GroupTreeItem*)m_treectrl->GetItemData(id);
	if (!data) {
		return;
	}

	if (data->m_sprite) {
		data->m_sprite->editable = !data->m_sprite->editable;
	}
}

//////////////////////////////////////////////////////////////////////////
// class SetVisibleVisitor
//////////////////////////////////////////////////////////////////////////

void GroupTreeImpl::SetVisibleVisitor::
VisitLeaf(wxTreeItemId id)
{
	assert(id.IsOk());
	GroupTreeItem* data = (GroupTreeItem*)m_treectrl->GetItemData(id);
	if (!data) {
		return;
	}

	if (data->m_sprite) {
		data->m_sprite->visiable = m_visible;
	}
}

//////////////////////////////////////////////////////////////////////////
// class SetEditableVisitor
//////////////////////////////////////////////////////////////////////////

void GroupTreeImpl::SetEditableVisitor::
VisitLeaf(wxTreeItemId id)
{
	assert(id.IsOk());
	GroupTreeItem* data = (GroupTreeItem*)m_treectrl->GetItemData(id);
	if (!data) {
		return;
	}

	if (data->m_sprite) {
		data->m_sprite->editable = m_editable;
	}
}

//////////////////////////////////////////////////////////////////////////
// class StoreVisitor
//////////////////////////////////////////////////////////////////////////

GroupTreeImpl::StoreVisitor::
StoreVisitor(const GroupTreeCtrl* treectrl, Json::Value& value)
	: m_treectrl(treectrl)
	, m_value(value)
{
}

void GroupTreeImpl::StoreVisitor::
VisitNonleaf(wxTreeItemId id)
{
	assert(id.IsOk());

	Json::Value val;
	val["leaf"] = false;

	val["name"] = GetName(id);
	val["parent"] = GetParentName(id);

	int sz = m_value["node"].size();
	m_value["node"][sz++] = val;
}

void GroupTreeImpl::StoreVisitor::
VisitLeaf(wxTreeItemId id)
{
	assert(id.IsOk());

	Json::Value val;
	val["leaf"] = true;

	val["name"] = GetName(id);
	val["parent"] = GetParentName(id);

	GroupTreeItem* data = (GroupTreeItem*)m_treectrl->GetItemData(id);
	if (data && data->m_sprite) {
		val["sprite"] = data->m_sprite->name;
	}

	int sz = m_value["node"].size();
	m_value["node"][sz++] = val;
}

std::string GroupTreeImpl::StoreVisitor::
GetName(wxTreeItemId id) const
{
	if (m_treectrl->GetRootID() == id) {
		return "root";
	} else {
		return m_treectrl->GetItemText(id).ToStdString();
	}
}

std::string GroupTreeImpl::StoreVisitor::
GetParentName(wxTreeItemId id) const
{
	if (m_treectrl->GetRootID() == id) {
		return "null";
	} else {
		return GetName(m_treectrl->GetItemParent(id));
	}
}

//////////////////////////////////////////////////////////////////////////
// class CopyPasteVisitor
//////////////////////////////////////////////////////////////////////////

GroupTreeImpl::CopyPasteVisitor::
CopyPasteVisitor(GroupTreeCtrl* treectrl, wxTreeItemId from, 
				 wxTreeItemId to)
	: m_treectrl(treectrl)
	, m_from(from)
	, m_to(to)
{
	m_map_ids.insert(std::make_pair(m_from, m_to));
}

void GroupTreeImpl::CopyPasteVisitor::
VisitNonleaf(wxTreeItemId id)
{
	CopyPaste(id);
}

void GroupTreeImpl::CopyPasteVisitor::
VisitLeaf(wxTreeItemId id)
{
	CopyPaste(id);
}

void GroupTreeImpl::CopyPasteVisitor::
CopyPaste(wxTreeItemId id)
{
	wxTreeItemId old_parent = m_treectrl->GetItemParent(id);
	std::map<wxTreeItemId, wxTreeItemId>::iterator itr 
		= m_map_ids.find(old_parent);
	wxTreeItemId new_parent;
	if (itr == m_map_ids.end()) {
		int zz = 0;
	} else {
		new_parent = itr->second;
	}

	GroupTreeItem* data = (GroupTreeItem*)m_treectrl->GetItemData(id);
	std::string name = 	m_treectrl->GetItemText(id);
	wxTreeItemId new_item = m_treectrl->AppendItem(new_parent, name, -1, -1, data);
	m_map_ids.insert(std::make_pair(id, new_item));
}

}