#include "LayersPanel.h"
#include "LayersPropertyWidget.h"
#include "LayersContentWidget.h"
#include "KeysPanel.h"
#include "StagePanel.h"

#include "frame/Controller.h"

namespace eanim
{

LayersPanel::LayersPanel(wxWindow* parent, Controller* ctrl)
	: wxPanel(parent)
	, m_ctrl(ctrl)
{
	initLayout();
	insertLayer();
}

void LayersPanel::insertLayer()
{
	LayersMgr& layers = m_ctrl->GetLayers();

	m_ctrl->setCurrFrame(layers.size(), m_ctrl->frame());
	layers.newLayer();

	m_parent->Refresh();
}

void LayersPanel::removeLayer()
{
	LayersMgr& layers = m_ctrl->GetLayers();

	m_ctrl->GetStagePanel()->removeSpriteSelection();

	layers.removeLayer(m_ctrl->layer());
	if (layers.size() == 0) 
	{
		m_ctrl->setCurrFrame(-1, m_ctrl->frame());
	}
	else if (m_ctrl->layer() > 0) 
	{
		m_ctrl->setCurrFrame(m_ctrl->layer() - 1, m_ctrl->frame());
	}
	m_ctrl->GetKeysPanel()->setCurrPos(m_ctrl->frame());
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