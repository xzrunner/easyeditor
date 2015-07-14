#include "GroupTreeImpl.h"
#include "GroupTreeItem.h"
#include "GroupTreeCtrl.h"

#include "dataset/Group.h"
#include "view/SpriteSelection.h"

// debug
#include <fstream>

namespace d2d
{

//////////////////////////////////////////////////////////////////////////
// class QuerySpriteVisitor
//////////////////////////////////////////////////////////////////////////

GroupTreeImpl::QuerySpriteVisitor::
QuerySpriteVisitor(wxTreeCtrl* treectrl, ISprite* spr)
	: m_treectrl(treectrl)
	, m_spr(spr)
{
	if (m_spr) {
		m_spr->Retain();
	}
}

GroupTreeImpl::QuerySpriteVisitor::
~QuerySpriteVisitor()
{
	if (m_spr) {
		m_spr->Release();
	}
}

bool GroupTreeImpl::QuerySpriteVisitor::
VisitLeaf(wxTreeItemId id)
{
	assert(id.IsOk());
	GroupTreeItem* data = (GroupTreeItem*)m_treectrl->GetItemData(id);
	if (!data || data->IsGroup()) {
		return false;
	}

	ISprite* spr = static_cast<GroupTreeSpriteItem*>(data)->GetSprite();
	if (spr == m_spr) {
		m_id = id;
		return true;
	}

	return false;
}
	
//////////////////////////////////////////////////////////////////////////
// class RemoveVisitor
//////////////////////////////////////////////////////////////////////////

GroupTreeImpl::RemoveVisitor::
RemoveVisitor(wxTreeCtrl* treectrl, ISprite* spr)
	: m_treectrl(treectrl)
	, m_spr(spr)
	, m_finish(false)
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

bool GroupTreeImpl::RemoveVisitor::
VisitNonleaf(wxTreeItemId id)
{
	assert(id.IsOk());

	GroupTreeItem* data = (GroupTreeItem*)m_treectrl->GetItemData(id);
	if (!data || !data->IsGroup()) {
		return false;
	}

	Group* group = static_cast<GroupTreeGroupItem*>(data)->GetGroup();
	bool finish = group->Remove(m_spr);
	if (finish) {
		m_finish = true;
	}

	return false;
}

bool GroupTreeImpl::RemoveVisitor::
VisitLeaf(wxTreeItemId id)
{
	if (!id.IsOk()) {
		return false;
	}

	GroupTreeItem* data = (GroupTreeItem*)m_treectrl->GetItemData(id);
	if (!data || data->IsGroup()) {
		return false;
	}

	ISprite* spr = static_cast<GroupTreeSpriteItem*>(data)->GetSprite();
	if (spr == m_spr) {
		m_treectrl->Delete(id);
		return true;
	}

	return false;
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

bool GroupTreeImpl::SelectVisitor::
VisitLeaf(wxTreeItemId id)
{
	assert(id.IsOk());
	GroupTreeItem* data = (GroupTreeItem*)m_treectrl->GetItemData(id);
	if (!data || data->IsGroup()) {
		return false;
	}

	ISprite* spr = static_cast<GroupTreeSpriteItem*>(data)->GetSprite();
	m_selection->Add(spr);

	return false;
}

//////////////////////////////////////////////////////////////////////////
// class GetSpritesVisitor
//////////////////////////////////////////////////////////////////////////

GroupTreeImpl::GetSpritesVisitor::
GetSpritesVisitor(wxTreeCtrl* treectrl, std::vector<ISprite*>& sprites)
	: m_treectrl(treectrl)
	, m_sprites(sprites)
{
}

bool GroupTreeImpl::GetSpritesVisitor::
VisitLeaf(wxTreeItemId id)
{
	assert(id.IsOk());
	GroupTreeItem* data = (GroupTreeItem*)m_treectrl->GetItemData(id);
	if (!data || data->IsGroup()) {
		return false;
	}

	ISprite* spr = static_cast<GroupTreeSpriteItem*>(data)->GetSprite();
	m_sprites.push_back(spr);

	return false;
}

//////////////////////////////////////////////////////////////////////////
// class GetFirstSpriteVisitor
//////////////////////////////////////////////////////////////////////////

GroupTreeImpl::GetFirstSpriteVisitor::
GetFirstSpriteVisitor(wxTreeCtrl* treectrl)
	: m_treectrl(treectrl)
	, m_first(NULL)
{
}

bool GroupTreeImpl::GetFirstSpriteVisitor::
VisitLeaf(wxTreeItemId id)
{
	assert(id.IsOk());
	GroupTreeItem* data = (GroupTreeItem*)m_treectrl->GetItemData(id);
	if (!data || data->IsGroup()) {
		return false;
	}

	m_first = static_cast<GroupTreeSpriteItem*>(data)->GetSprite();

	return true;
}

//////////////////////////////////////////////////////////////////////////
// class VisibleVisitor
//////////////////////////////////////////////////////////////////////////

bool GroupTreeImpl::VisibleVisitor::
VisitLeaf(wxTreeItemId id)
{
	assert(id.IsOk());
	GroupTreeItem* data = (GroupTreeItem*)m_treectrl->GetItemData(id);
	if (!data) {
		return false;
	}

	if (data->IsGroup()) {
		Group* group = static_cast<GroupTreeGroupItem*>(data)->GetGroup();
		group->SetVisible(!group->GetVisible());
	} else {
	 	ISprite* spr = static_cast<GroupTreeSpriteItem*>(data)->GetSprite();
	 	spr->visiable = !spr->visiable;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// class EditableVisitor
//////////////////////////////////////////////////////////////////////////

bool GroupTreeImpl::EditableVisitor::
VisitLeaf(wxTreeItemId id)
{
	assert(id.IsOk());
	GroupTreeItem* data = (GroupTreeItem*)m_treectrl->GetItemData(id);
	if (!data) {
		return false;
	}

	if (data->IsGroup()) {
		Group* group = static_cast<GroupTreeGroupItem*>(data)->GetGroup();
		group->SetEditable(!group->GetEditable());
	} else {
		ISprite* spr = static_cast<GroupTreeSpriteItem*>(data)->GetSprite();
		spr->editable = !spr->editable;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// class SetVisibleVisitor
//////////////////////////////////////////////////////////////////////////

bool GroupTreeImpl::SetVisibleVisitor::
VisitLeaf(wxTreeItemId id)
{
	assert(id.IsOk());
	GroupTreeItem* data = (GroupTreeItem*)m_treectrl->GetItemData(id);
	if (data->IsGroup()) {
		Group* group = static_cast<GroupTreeGroupItem*>(data)->GetGroup();
		group->SetVisible(m_visible);
	} else {
		ISprite* spr = static_cast<GroupTreeSpriteItem*>(data)->GetSprite();
		spr->visiable = m_visible;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// class SetEditableVisitor
//////////////////////////////////////////////////////////////////////////

bool GroupTreeImpl::SetEditableVisitor::
VisitLeaf(wxTreeItemId id)
{
	assert(id.IsOk());
	GroupTreeItem* data = (GroupTreeItem*)m_treectrl->GetItemData(id);
	if (!data) {
		return false;
	}

	if (data->IsGroup()) {
		Group* group = static_cast<GroupTreeGroupItem*>(data)->GetGroup();
		group->SetEditable(m_editable);
	} else {
		ISprite* spr = static_cast<GroupTreeSpriteItem*>(data)->GetSprite();
		spr->editable = m_editable;
	}
	return false;
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

bool GroupTreeImpl::StoreVisitor::
VisitNonleaf(wxTreeItemId id)
{
	assert(id.IsOk());

	Json::Value val;
	val["leaf"] = false;

	val["name"] = GetName(id);
	val["parent"] = GetParentName(id);

	GroupTreeItem* data = (GroupTreeItem*)m_treectrl->GetItemData(id);
	assert(data && data->IsGroup());
	Group* group = static_cast<GroupTreeGroupItem*>(data)->GetGroup();
	val["visible"] = group->GetVisible();
	val["editable"] = group->GetEditable();

	int sz = m_value["node"].size();
	m_value["node"][sz++] = val;

	return false;
}

bool GroupTreeImpl::StoreVisitor::
VisitLeaf(wxTreeItemId id)
{
	assert(id.IsOk());

	Json::Value val;
	val["leaf"] = true;

	val["name"] = GetName(id);
	val["parent"] = GetParentName(id);

	GroupTreeItem* data = (GroupTreeItem*)m_treectrl->GetItemData(id);
	if (m_treectrl->GetRootID() == id) {
		;
	} else {
		assert(data && !data->IsGroup());
		ISprite* spr = static_cast<GroupTreeSpriteItem*>(data)->GetSprite();
		if (spr) {
			val["sprite"] = spr->name;
		} else {
			// fixme
			int zz = 0;
		}
	}

	int sz = m_value["node"].size();
	m_value["node"][sz++] = val;

	return false;
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
	m_map_ids.insert(std::make_pair(m_treectrl->GetItemParent(m_from), m_treectrl->GetItemParent(m_to)));	
	m_map_ids.insert(std::make_pair(m_from, m_to));
}

bool GroupTreeImpl::CopyPasteVisitor::
VisitNonleaf(wxTreeItemId id)
{
	CopyPaste(id);
	return false;
}

bool GroupTreeImpl::CopyPasteVisitor::
VisitLeaf(wxTreeItemId id)
{
	CopyPaste(id);
	return false;
}

void GroupTreeImpl::CopyPasteVisitor::
CopyPaste(wxTreeItemId id)
{
	if (m_map_ids.find(id) != m_map_ids.end()) {
		return;
	}

	wxString str = m_treectrl->GetItemText(id);

	wxTreeItemId old_parent = m_treectrl->GetItemParent(id);
	wxString str_p = m_treectrl->GetItemText(old_parent);

	std::map<wxTreeItemId, wxTreeItemId>::iterator itr 
		= m_map_ids.find(old_parent);
	assert(itr != m_map_ids.end());
	wxTreeItemId new_parent = itr->second;

	wxString str_p_n;
	if (new_parent != m_treectrl->GetRootID()) {
		str_p_n = m_treectrl->GetItemText(new_parent);
	}

	GroupTreeItem* data = (GroupTreeItem*)m_treectrl->GetItemData(id);
	std::string name = 	m_treectrl->GetItemText(id);
	wxTreeItemId new_item = m_treectrl->AppendItem(new_parent, name, -1, -1, data->Clone());
	m_treectrl->ExpandAll();
	m_map_ids.insert(std::make_pair(id, new_item));
}

}