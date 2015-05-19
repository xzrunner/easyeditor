#include "GroupTreeImpl.h"
#include "GroupTreeItem.h"

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
// class StoreVisitor
//////////////////////////////////////////////////////////////////////////

GroupTreeImpl::StoreVisitor::
StoreVisitor(const wxTreeCtrl* treectrl, Json::Value& value)
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

	val["name"] = m_treectrl->GetItemText(id).ToStdString();
	val["parent"] = m_treectrl->GetItemText(m_treectrl->GetItemParent(id)).ToStdString();

	int sz = m_value["node"].size();
	m_value["node"][sz++] = val;
}

void GroupTreeImpl::StoreVisitor::
VisitLeaf(wxTreeItemId id)
{
	assert(id.IsOk());

	Json::Value val;
	val["leaf"] = true;

	val["name"] = m_treectrl->GetItemText(id).ToStdString();
	val["parent"] = m_treectrl->GetItemText(m_treectrl->GetItemParent(id)).ToStdString();

	GroupTreeItem* data = (GroupTreeItem*)m_treectrl->GetItemData(id);
	if (data && data->m_sprite) {
		val["sprite"] = data->m_sprite->name;
	}

	int sz = m_value["node"].size();
	m_value["node"][sz++] = val;
}

}