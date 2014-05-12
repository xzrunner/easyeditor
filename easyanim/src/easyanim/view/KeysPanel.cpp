#include "KeysPanel.h"
#include "TimeLinePanel.h"
#include "StagePanel.h"
#include "KeysScaleWidget.h"
#include "KeysContentWidget.h"
#include "LayersPanel.h"

#include "frame/Context.h"
#include "dataset/Layer.h"
#include "dataset/LayersMgr.h"

namespace eanim
{

KeysPanel::KeysPanel(wxWindow* parent, LayersMgr* layersMgr)
	: wxPanel(parent)
{
	m_layersMgr = layersMgr;
	m_currPos = 1;
	m_selectRow = m_selectCol = -1;
	initLayout();
}

int KeysPanel::getCurrPos() const
{
	return m_currPos;
}

void KeysPanel::setCurrPos(int pos)
{
	int maxPos = 1;
	for (size_t i = 0, n = m_layersMgr->size(); i < n; ++i)
	{
		const std::map<int, KeyFrame*>& frames = m_layersMgr->getLayer(i)->getAllFrames();
		if (!frames.empty())
			maxPos = std::max(maxPos, (--frames.end())->first);
	}

	if (pos > maxPos) pos = maxPos;
	if (pos != m_currPos)
	{
		m_currPos = pos;
		m_scaleWidget->Refresh();
		m_contentWidget->Refresh();
		Context::Instance()->stage->loadCurrFrameSprites();
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
		if (row != -1) Context::Instance()->timeline->getLayersPanel()->setSelectedLayer(m_layersMgr->size() - row - 1);
		refresh = true;
	}
	if (col != m_selectCol)
	{
		m_selectCol = col;
		if (col != -1) setCurrPos(col + 1);
		refresh = true;
	}
	if (refresh) m_contentWidget->Refresh();
}

void KeysPanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	m_scaleWidget = new KeysScaleWidget(this, wxID_ANY);
	sizer->Add(m_scaleWidget, 0, wxEXPAND);
	m_contentWidget = new KeysContentWidget(this, wxID_ANY, m_layersMgr);
	sizer->Add(m_contentWidget, 1, wxEXPAND);
	SetSizer(sizer);
}

} // eanim