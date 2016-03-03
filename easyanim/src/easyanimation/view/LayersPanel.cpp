#include "LayersPanel.h"
#include "LayersPropertyWidget.h"
#include "LayersContentWidget.h"
#include "KeysPanel.h"
#include "StagePanel.h"

#include "dataset/Layer.h"

namespace eanim
{

LayersPanel::LayersPanel(wxWindow* parent)
	: wxScrolledWindow(parent)
{
	InitLayout();
}

//void LayersPanel::onPlay()
//{
// 	KeysPanel* keysPanel = m_timeLinePanel->getKeysPanel();
// 	int last = keysPanel->getCurrPos();
// 	do 
// 	{
// 		keysPanel->setCurrPos(++last);
// //		Sleep(1000.0f / 24);
// 	} while (keysPanel->getCurrPos() == last);
//}

void LayersPanel::InitLayout()
{
	SetScrollbars(0, 1, 0, 10, 0, 0);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(m_layersProperty = new LayersPropertyWidget(this), 1, wxEXPAND);
	sizer->Add(m_contentPanel = new LayersContentWidget(this), 99, wxEXPAND);

	SetSizer(sizer);
}

} // eanim