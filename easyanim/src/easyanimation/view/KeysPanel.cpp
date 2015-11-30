#include "KeysPanel.h"
#include "StagePanel.h"
#include "KeysScaleWidget.h"
#include "KeysContentWidget.h"

#include "frame/Controller.h"
#include "dataset/Layer.h"
#include "dataset/LayersMgr.h"
#include "message/GetCurrFrameSJ.h"

namespace eanim
{

KeysPanel::KeysPanel(wxWindow* parent, Controller* ctrl)
	: wxScrolledWindow(parent)
	, m_ctrl(ctrl)
{
	m_selected_row = m_selected_col = -1;
	m_selected_col_min = m_selected_col_max = -1;

	InitLayout();
}

void KeysPanel::GetSelectPos(int& row, int& col) const
{
	row = m_selected_row;
	col = m_selected_col;
}

void KeysPanel::SetSelectPos(int row, int col)
{
	int max_frame = -1;

	LayersMgr& layers = m_ctrl->GetLayers();
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

	if (row == m_selected_row && col == m_selected_col) {
		return;
	}

	int layer, frame;
	GetCurrFrameSJ::Instance()->Get(layer, frame);

	if (row != m_selected_row) {
		m_selected_row = row;
		if (row != -1) {
			layer = m_ctrl->GetLayers().Size() - row - 1;
		}
	}

	if (col != m_selected_col) {
		m_selected_col = col;
		if (col != -1) {
			frame = col + 1;
		}
	}

	if (col + 1 > max_frame) {
		SetCurrFrameSJ::Instance()->Set(layer, max_frame);
		m_selected_col = col;
	} else {
		SetCurrFrameSJ::Instance()->Set(layer, frame);
	}

	m_ctrl->UpdateCurrFrame();

	Refresh(true);
}

void KeysPanel::GetSelectRegion(int& row, int& col_min, int& col_max)
{
	row = m_selected_row;
	col_min = m_selected_col_min;
	col_max = m_selected_col_max;
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

	int layer, frame;
	GetCurrFrameSJ::Instance()->Get(layer, frame);
	m_selected_col_max = std::min(m_selected_col_max, 
		m_ctrl->GetLayers().GetLayer(layer)->GetMaxFrameTime() - 1);

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

	int layer, frame;
	GetCurrFrameSJ::Instance()->Get(layer, frame);
	m_selected_col_max = std::min(m_selected_col_max, 
		m_ctrl->GetLayers().GetLayer(layer)->GetMaxFrameTime() - 1);

	Refresh(true);
}

void KeysPanel::InitLayout()
{
	SetScrollbars(1,1, 200, 100, 0, 0);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(new KeysScaleWidget(this, m_ctrl), 0, wxEXPAND);
	sizer->Add(new KeysContentWidget(this, m_ctrl), 1, wxEXPAND);

	wxBoxSizer* horSizer = new wxBoxSizer(wxHORIZONTAL);
	horSizer->AddSpacer(6000);
	sizer->Add(horSizer);

	SetSizer(sizer);
}

} // eanim