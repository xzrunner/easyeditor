#include "KeysPanel.h"
#include "StagePanel.h"
#include "KeysScaleWidget.h"
#include "KeysContentWidget.h"

#include "dataset/KeyFrame.h"
#include "dataset/Layer.h"
#include "dataset/LayersMgr.h"
#include "dataset/DataMgr.h"
#include "message/messages.h"
#include "view/ViewMgr.h"

#include <ee/ViewlistPanel.h>

namespace eanim
{

KeysPanel::KeysPanel(wxWindow* parent)
	: wxScrolledWindow(parent)
	, m_layer(NULL)
{
	m_selected_row = m_selected_col = -1;

	InitLayout();

	RegistSubject(SetSelectedSJ::Instance());
}

void KeysPanel::GetSelectPos(int& row, int& col) const
{
	row = m_selected_row;
	col = m_selected_col;
}

void KeysPanel::OnNotify(int sj_id, void* ud)
{
	switch (sj_id)
	{
	case MSG_SET_CURR_FRAME:
		{
			SetSelectedSJ::Position* cf = (SetSelectedSJ::Position*)ud;
			if (cf->layer == -1 && cf->frame == -1) {
				m_selected_row = m_selected_col = -1;
				m_layer = NULL;
				ViewMgr::Instance()->viewlist->Clear();
				return;
			}

			int row = cf->layer == -1 ? m_selected_row : DataMgr::Instance()->GetLayers().Size() - 1 - cf->layer;
			int col = cf->frame == -1 ? m_selected_col : cf->frame;
			if (row == -1) {
				return;
			}
			if (row == m_selected_row && col == m_selected_col) {
				return;
			}

			if (row != m_selected_row) {
				m_selected_row = row;
				assert(cf->layer < DataMgr::Instance()->GetLayers().Size());
				m_layer = DataMgr::Instance()->GetLayers().GetLayer(cf->layer);
			}
			if (col != m_selected_col) {
				m_selected_col = col;
			}
		}
		break;
	}
}

void KeysPanel::InitLayout()
{
	SetScrollbars(1, 1, 20, 10, 0, 0);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(new KeysScaleWidget(this), 1, wxEXPAND);
	sizer->Add(new KeysContentWidget(this), 99, wxEXPAND);

	wxBoxSizer* hori_sizer = new wxBoxSizer(wxHORIZONTAL);
	hori_sizer->AddSpacer(2000);	// todo
	sizer->Add(hori_sizer);

// 	wxBoxSizer* vert_sizer = new wxBoxSizer(wxVERTICAL);
// 	vert_sizer->AddSpacer(50);
// 	sizer->Add(vert_sizer);

	SetSizer(sizer);
}

} // eanim