#include "KeysPanel.h"
#include "StagePanel.h"
#include "KeysScaleWidget.h"
#include "KeysContentWidget.h"

#include "dataset/Layer.h"
#include "dataset/LayersMgr.h"
#include "dataset/DataMgr.h"
#include "dataset/data_utility.h"
#include "view/view_utility.h"
#include "message/messages.h"

namespace eanim
{

KeysPanel::KeysPanel(wxWindow* parent)
	: wxScrolledWindow(parent)
{
	m_selected_row = m_selected_col = -1;
	m_selected_col_min = m_selected_col_max = -1;

	InitLayout();

	RegistSubject(SetCurrFrameSJ::Instance());
	RegistSubject(SetSelectedRegionSJ::Instance());
	RegistSubject(UpdateSelectedRegionSJ::Instance());
}

void KeysPanel::GetSelectPos(int& row, int& col) const
{
	row = m_selected_row;
	col = m_selected_col;
}

void KeysPanel::GetSelectRegion(int& row, int& col_min, int& col_max)
{
	row = m_selected_row;
	col_min = m_selected_col_min;
	col_max = m_selected_col_max;
}

void KeysPanel::OnNotify(int sj_id, void* ud)
{
	switch (sj_id)
	{
	case MSG_SET_CURR_FRAME:
		{
			SetCurrFrameSJ::CurrFrame* cf = (SetCurrFrameSJ::CurrFrame*)ud;
			SetSelectPos(DataMgr::Instance()->GetLayers().Size() - 1 - cf->layer, cf->frame);
		}
		break;
	case MSG_SET_SELECTED_REGION:
		{
			SetSelectedRegionSJ::Region* r = (SetSelectedRegionSJ::Region*)ud;
			SetSelectRegion(r->row, r->col);
		}
		break;
	case MSG_UPDATE_SELECTED_REGION:
		{
			UpdateSelectedRegionSJ::Region* r = (UpdateSelectedRegionSJ::Region*)ud;
			UpdateSelectRegion(r->min, r->max);
		}
		break;
	}
}

void KeysPanel::InitLayout()
{
	SetScrollbars(1,1, 200, 100, 0, 0);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(new KeysScaleWidget(this), 0, wxEXPAND);
	sizer->Add(new KeysContentWidget(this), 1, wxEXPAND);

	wxBoxSizer* horSizer = new wxBoxSizer(wxHORIZONTAL);
	horSizer->AddSpacer(6000);
	sizer->Add(horSizer);

	SetSizer(sizer);
}

void KeysPanel::SetSelectPos(int row, int col)
{
	int max_frame = -1;

	LayersMgr& layers = DataMgr::Instance()->GetLayers();
	Layer* curr_layer = layers.GetLayer(layers.Size() - row - 1);
	if (curr_layer) {
		max_frame = curr_layer->GetMaxFrameTime();
	} else {
		return;
	}

	if (col + 1 > max_frame || m_selected_col > max_frame) {
		m_selected_col_min = m_selected_col_max = -1;
	} else {
		m_selected_col_min = m_selected_col_max = col;
	}

	if (row != m_selected_row || col != m_selected_col) {
		m_selected_row = row;
		m_selected_col = col;
		reload_viewlist(get_curr_frame());
	}
}

void KeysPanel::SetSelectRegion(int row, int col)
{
	if (m_selected_col == -1 || m_selected_row == -1 || m_selected_row != row) {
		return SetSelectPos(row, col);
	}

	if (m_selected_col < col) {
		m_selected_col_min = m_selected_col;
		m_selected_col_max = col;
	} else {
		m_selected_col_min = col;
		m_selected_col_max = m_selected_col;
	}

	int _col;
	Layer* layer = get_curr_layer(_col);
	if (layer) {
		m_selected_col_max = std::min(m_selected_col_max, layer->GetMaxFrameTime() - 1);
	}

	Refresh(true);
}

void KeysPanel::UpdateSelectRegion(int col_min, int col_max)
{
	if (m_selected_col_min == -1 || m_selected_col_max == -1 || 
		m_selected_col_min == col_min && m_selected_col_max == col_max) {
			return;
	}

	m_selected_col_min = col_min;
	m_selected_col_max = col_max;

	int col;
	Layer* layer = get_curr_layer(col);
	if (layer) {
		m_selected_col_max = std::min(m_selected_col_max, layer->GetMaxFrameTime() - 1);
	}

	Refresh(true);
}

} // eanim