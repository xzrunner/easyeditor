#include "LayersPanel.h"
#include "LayersPropertyWidget.h"
#include "LayersContentWidget.h"
#include "KeysPanel.h"
#include "StagePanel.h"

#include "dataset/Layer.h"
#include "frame/Controller.h"
#include "message/InsertLayerSJ.h"

namespace eanim
{

LayersPanel::LayersPanel(wxWindow* parent, Controller* ctrl)
	: wxPanel(parent)
	, m_ctrl(ctrl)
{
	initLayout();

	InsertLayerSJ::Instance()->Insert();
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

void LayersPanel::setFPS(int fps)
{
	m_layersProperty->setFPS(fps);
}

void LayersPanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(m_layersProperty = new LayersPropertyWidget(this, m_ctrl), 0, wxALIGN_LEFT);
	sizer->Add(m_contentPanel = new LayersContentWidget(this, m_ctrl), 1, wxEXPAND);
	SetSizer(sizer);
}

} // eanim