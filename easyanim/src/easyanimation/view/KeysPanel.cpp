#include "KeysPanel.h"
//#include "TimeLinePanel.h"
#include "StagePanel.h"
#include "KeysScaleWidget.h"
#include "KeysContentWidget.h"
#include "LayersPanel.h"

#include "frame/Controller.h"
#include "dataset/Layer.h"
#include "dataset/LayersMgr.h"

namespace eanim
{

KeysPanel::KeysPanel(wxWindow* parent, Controller* ctrl)
	: wxScrolledWindow(parent)
	, m_ctrl(ctrl)
{
	m_ctrl->setCurrFrame(m_ctrl->layer(), 1);

	m_selectRow = m_selectCol = -1;
	initLayout();
}

void KeysPanel::setCurrPos(int pos)
{
	int maxFrame = 1;
	LayersMgr& layers = m_ctrl->GetLayers();
	for (size_t i = 0, n = layers.size(); i < n; ++i)
	{
		const std::map<int, KeyFrame*>& frames = layers.getLayer(i)->getAllFrames();
		if (!frames.empty())
			maxFrame = std::max(maxFrame, (--frames.end())->first);
	}

	if (pos > maxFrame) pos = maxFrame;
	if (pos != m_ctrl->frame())
	{
		m_ctrl->setCurrFrame(m_ctrl->layer(), pos);
		m_ctrl->reloadViewList(m_ctrl->getCurrFrame());
//		static_cast<StagePanel*>(context->stage)->clear();
		m_ctrl->GetStagePanel()->getEditOP()->clear();
		Refresh();
	}
}

void KeysPanel::getSelectPos(int& row, int& col) const
{
	row = m_selectRow;
	col = m_selectCol;
}

void KeysPanel::setSelectPos(int row, int col)
{
	bool refresh = false;
	if (row != m_selectRow)
	{
		m_selectRow = row;
		if (row != -1) 
		{
			int layer = m_ctrl->GetLayers().size() - row - 1;
			m_ctrl->setCurrFrame(layer, m_ctrl->frame());
		}
		refresh = true;
	}
	if (col != m_selectCol)
	{
		m_selectCol = col;
		if (col != -1) setCurrPos(col + 1);
		refresh = true;
	}
	if (refresh) {
		m_ctrl->Refresh();
	}
}

void KeysPanel::initLayout()
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