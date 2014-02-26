#include "KeysPanel.h"
//#include "TimeLinePanel.h"
#include "StagePanel.h"
#include "KeysScaleWidget.h"
#include "KeysContentWidget.h"
#include "LayersPanel.h"

#include "frame/Context.h"
#include "dataset/Layer.h"
#include "dataset/LayersMgr.h"

namespace eanim
{

KeysPanel::KeysPanel(wxWindow* parent)
	: wxScrolledWindow(parent)
{
	Context* context = Context::Instance();
	context->setCurrFrame(context->layer(), 1);

	m_selectRow = m_selectCol = -1;
	initLayout();
}

void KeysPanel::setCurrPos(int pos)
{
	int maxFrame = 1;
	Context* context = Context::Instance();
	for (size_t i = 0, n = context->layers.size(); i < n; ++i)
	{
		const std::map<int, KeyFrame*>& frames = context->layers.getLayer(i)->getAllFrames();
		if (!frames.empty())
			maxFrame = std::max(maxFrame, (--frames.end())->first);
	}

	if (pos > maxFrame) pos = maxFrame;
	if (pos != context->frame())
	{
		context->setCurrFrame(context->layer(), pos);
//		static_cast<StagePanel*>(context->stage)->clear();
		static_cast<StagePanel*>(context->stage)->getEditOP()->clear();
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
			Context* context = Context::Instance();
			int layer = context->layers.size() - row - 1;
			context->setCurrFrame(layer, context->frame());
		}
		refresh = true;
	}
	if (col != m_selectCol)
	{
		m_selectCol = col;
		if (col != -1) setCurrPos(col + 1);
		refresh = true;
	}
	if (refresh) 
	{
		Context::Instance()->stage->Refresh();
		Context::Instance()->keysPanel->Refresh();
	}
}

void KeysPanel::initLayout()
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

} // eanim