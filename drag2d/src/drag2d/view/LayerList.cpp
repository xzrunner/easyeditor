#include "LayerList.h"

#include "dataset/Layer.h"
#include "dataset/LayerMgr.h"
#include "message/subject_id.h"
#include "message/panel_msg.h"
#include "message/SetCanvasDirtySJ.h"

#include <vector>

namespace d2d
{

static const int LAYER_LIST_ID = 3000;

wxBEGIN_EVENT_TABLE(LayerList, wxListCtrl)
	EVT_LIST_END_LABEL_EDIT(LAYER_LIST_ID, LayerList::OnEndLabelEdit)
	EVT_LIST_ITEM_SELECTED(LAYER_LIST_ID, LayerList::OnSelected)
	EVT_LIST_COL_CLICK(LAYER_LIST_ID, LayerList::OnColClick)
	EVT_LIST_KEY_DOWN(LAYER_LIST_ID, LayerList::OnListKeyDown)
wxEND_EVENT_TABLE()

static const char* STAT_SELECTED = "[!]";

LayerList::LayerList(wxWindow* parent)
	: wxListCtrl(parent, LAYER_LIST_ID, wxDefaultPosition, wxDefaultSize, 
	wxLC_REPORT | wxLC_EDIT_LABELS | wxLC_SINGLE_SEL)
	, m_layer_mgr(NULL)
{
	SetBackgroundColour(wxColour(229, 229, 229));
	InitLayout();

	RegistSubject(ChangeLayerMgrSJ::Instance());
}

void LayerList::Insert()
{
	if (!m_layer_mgr) {
		return;
	}

	Layer* layer = new Layer;
	m_layer_mgr->Insert(layer);
	Insert(layer);
	layer->Release();
}

void LayerList::Remove()
{
	if (!m_layer_mgr) {
		return;
	}

	if (m_layer_mgr->selected) {
		Remove(m_layer_mgr->selected);
	}

	if (GetItemCount() > 0) {
		m_layer_mgr->selected = m_layer_mgr->GetLayer(GetItemCount() - 1);
		SetItem(0, 1, STAT_SELECTED);
	}
}

void LayerList::OnNotify(int sj_id, void* ud)
{
	switch (sj_id) 
	{
	case MSG_CHANGE_LAYER_MGR_MSG:
		{
			m_layer_mgr = (d2d::LayerMgr*)ud;
			LoadFromLayerMgr(m_layer_mgr);
		}
		break;
	}
}

void LayerList::InitLayout()
{
	InsertColumn(0, "name", wxLIST_FORMAT_LEFT, 100);
	InsertColumn(1, "stat", wxLIST_FORMAT_LEFT, 40);
	InsertColumn(2, "view", wxLIST_FORMAT_LEFT, 50);
	InsertColumn(3, "edit", wxLIST_FORMAT_LEFT, 50);
}

void LayerList::Insert(Layer* layer)
{
	long item = InsertItem(0, "");

	SetItem(item, 0, layer->name);

	std::string stat = m_layer_mgr->selected == layer ? STAT_SELECTED : "";
	SetItem(item, 1, stat);

	SetItem(item, 2, layer->visible ? "T" : "F");
	SetItem(item, 3, layer->editable ? "T" : "F");
}

void LayerList::Remove(Layer* layer)
{
	const std::vector<Layer*>& layers = m_layer_mgr->GetAllLayers();
	for (int i = 0, n = layers.size(); i < n; ++i) {
		if (layer == layers[i]) {
			m_layer_mgr->Remove(i);
			DeleteItem(n - 1 - i);
			return;
		}
	}
}

void LayerList::LoadFromLayerMgr(LayerMgr* layer_mgr)
{
	DeleteAllItems();

	const std::vector<Layer*>& layers = layer_mgr->GetAllLayers();
	for (int i = 0, n = layers.size(); i < n; ++i) {
		Insert(layers[i]);
	}
}

void LayerList::OnEndLabelEdit(wxListEvent& event)
{
	int item = event.GetIndex();
	Layer* layer = m_layer_mgr->GetLayer(GetItemCount() - 1 - item);
	if (layer) {
		layer->name = event.m_item.m_text;
	}
}

void LayerList::OnSelected(wxListEvent& event)
{
	for (int i = 0, n = GetItemCount(); i < n; ++i) {
		SetItem(i, 1, "");
	}
	int item = event.GetIndex();
	SetItem(item, 1, STAT_SELECTED);

	m_layer_mgr->selected = m_layer_mgr->GetLayer(GetItemCount() - 1 - item);
}

void LayerList::OnColClick(wxListEvent& event)
{
	int col = event.GetColumn();
	if (col != 2 && col != 3) {
		return;
	}

	const std::vector<Layer*>& layers = m_layer_mgr->GetAllLayers();
	if (layers.empty()) {
		return;
	}

	if (col == 2) 
	{
		bool visible = !layers[0]->visible;
		std::string tag = visible ? "T" : "F";
		for (int i = 0, n = layers.size(); i < n; ++i) {
			SetItem(i, 2, tag);
			layers[i]->visible = visible;
		}
		SetCanvasDirtySJ::Instance()->SetDirty();
	} 
	else 
	{
		assert(col == 3);
		bool editable = !layers[0]->editable;
		std::string tag = editable ? "T" : "F";
		for (int i = 0, n = layers.size(); i < n; ++i) {
			SetItem(i, 3, tag);
			layers[i]->editable = editable;
		}
	}
}

void LayerList::OnListKeyDown(wxListEvent& event)
{
	switch (event.GetKeyCode())
	{
	case WXK_DELETE:
		if (m_layer_mgr->selected) {
			Remove(m_layer_mgr->selected);
		}
		break;
	}
}

}