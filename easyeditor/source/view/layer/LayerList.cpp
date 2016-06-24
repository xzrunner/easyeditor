#include "LayerList.h"
#include "Layer.h"
#include "LayerMgr.h"
#include "LayerDialog.h"
#include "subject_id.h"
#include "panel_msg.h"
#include "DeleteLayerAOP.h"
#include "FetchAllVisitor.h"
#include "shape_msg.h"
#include "sprite_msg.h"

#include <vector>
#include <algorithm>

namespace ee
{

static const int LAYER_LIST_ID = 3000;

wxBEGIN_EVENT_TABLE(LayerList, wxListCtrl)
	EVT_LIST_END_LABEL_EDIT(LAYER_LIST_ID, LayerList::OnEndLabelEdit)
	EVT_LIST_ITEM_SELECTED(LAYER_LIST_ID, LayerList::OnSelected)
	EVT_LIST_COL_CLICK(LAYER_LIST_ID, LayerList::OnColClick)
	EVT_LIST_KEY_DOWN(LAYER_LIST_ID, LayerList::OnListKeyDown)
	EVT_LIST_ITEM_ACTIVATED(LAYER_LIST_ID, LayerList::OnActivated)
wxEND_EVENT_TABLE()

static const char* STAT_SELECTED = "[!]";

LayerList::LayerList(wxWindow* parent)
	: wxListCtrl(parent, LAYER_LIST_ID, wxDefaultPosition, wxDefaultSize, 
	wxLC_REPORT | wxLC_EDIT_LABELS | wxLC_SINGLE_SEL)
	, m_layer_mgr(NULL)
	, m_selected(0)
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
		m_selected = 0;
	}
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
			ClearLayer(layer);
			m_layer_mgr->Remove(i);
			DeleteItem(n - 1 - i);
			return;
		}
	}
}

void LayerList::OnNotify(int sj_id, void* ud)
{
	switch (sj_id) 
	{
	case MSG_CHANGE_LAYER_MGR_MSG:
		{
			m_layer_mgr = (LayerMgr*)ud;
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

void LayerList::LoadFromLayerMgr(LayerMgr* layer_mgr)
{
	DeleteAllItems();

	const std::vector<Layer*>& layers = layer_mgr->GetAllLayers();
	for (int i = 0, n = layers.size(); i < n; ++i) {
		Insert(layers[i]);
	}
}

void LayerList::ClearLayer(Layer* layer)
{
	EditAddRecordSJ::Instance()->Add(new DeleteLayerAOP(this, layer));

	std::vector<Sprite*> sprites;
	layer->TraverseSprite(FetchAllVisitor<Sprite>(sprites));
	for_each(sprites.begin(), sprites.end(), RetainObjectFunctor<Sprite>());
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		RemoveSpriteSJ::Instance()->Remove(sprites[i]);
	}

	std::vector<Shape*> shapes;
	layer->TraverseSprite(FetchAllVisitor<Shape>(shapes));
	for_each(shapes.begin(), shapes.end(), RetainObjectFunctor<Shape>());
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		RemoveShapeSJ::Instance()->Remove(shapes[i]);
	}

	SetCanvasDirtySJ::Instance()->SetDirty();
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
	m_selected = item;
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
	Layer* layer = m_layer_mgr->selected;
	if (!layer) { return; }

	int keycode = event.GetKeyCode();
	if (keycode == WXK_DELETE) {
		Remove(m_layer_mgr->selected);
	} else if (keycode == 'v' || keycode == 'V') {
		layer->visible = !layer->visible;
		std::string tag = layer->visible ? "T" : "F";
		SetItem(m_selected, 2, tag);
		SetCanvasDirtySJ::Instance()->SetDirty();
	} else if (keycode == 'e' || keycode == 'E') {
		layer->editable = !layer->editable;
		std::string tag = layer->editable ? "T" : "F";
		SetItem(m_selected, 3, tag);
	}

	event.Veto();
}

void LayerList::OnActivated(wxListEvent& event)
{
	int item = event.GetIndex();
	Layer* layer = m_layer_mgr->GetLayer(GetItemCount() - 1 - item);
	if (!layer) {
		return;
	}

	bool ori_view = layer->visible,
		 ori_edit = layer->editable;

	wxPoint pos = wxGetMousePosition();
	pos.x -= 400;

	LayerDialog dlg(this, layer, pos);
	dlg.ShowModal();

	if (ori_view != layer->visible) {
		std::string tag = layer->visible ? "T" : "F";
		SetItem(item, 2, tag);
		SetCanvasDirtySJ::Instance()->SetDirty();		
	}
	if (ori_edit != layer->editable) {
		std::string tag = layer->editable ? "T" : "F";
		SetItem(item, 3, tag);
	}
}

}