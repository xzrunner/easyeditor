#include "LayersPanel.h"
#include "LayersPropertyWidget.h"
#include "LayersContentWidget.h"
#include "TimeLinePanel.h"
#include "KeysPanel.h"
#include "Utility.h"
#include "StagePanel.h"

#include "frame/Context.h"
#include "dataset/LayersMgr.h"

namespace eanim
{

LayersPanel::LayersPanel(wxWindow* parent, LayersMgr* layersMgr)
	: wxPanel(parent)
{
	m_layersMgr = layersMgr;
	initLayout();

	m_selected = -1;
}

void LayersPanel::onAddLayer()
{
	m_selected = m_layersMgr->size();
	m_layersMgr->insertNewLayer();
	m_parent->Refresh();
}

void LayersPanel::onDelLayer()
{
	Context* context = Context::Instance();

	context->stage->removeSpriteSelection();
	m_layersMgr->deleteLayer(m_selected);
	if (m_layersMgr->size() == 0) m_selected = -1;
	else if (m_selected > 0) --m_selected;
	KeysPanel* keysPanel = context->timeline->getKeysPanel();
	keysPanel->setCurrPos(keysPanel->getCurrPos());
	context->timeline->Refresh();

	context->stage->loadCurrFrameSprites();
}

void LayersPanel::onPlay()
{
// 	KeysPanel* keysPanel = m_timeLinePanel->getKeysPanel();
// 	int last = keysPanel->getCurrPos();
// 	do 
// 	{
// 		keysPanel->setCurrPos(++last);
// //		Sleep(1000.0f / 24);
// 	} while (keysPanel->getCurrPos() == last);
}

void LayersPanel::setSelectedLayer(int index)
{
	if (m_selected != index && index < m_layersMgr->size())
	{
		m_selected = index;
		m_contentWidget->Refresh();
	}
}

int LayersPanel::getSelectedLayer() const
{
	return m_selected;
}

void LayersPanel::refreshKeysPanel() const
{
	Context::Instance()->timeline->getKeysPanel()->Refresh();
}

int LayersPanel::getFPSVal() const
{
	return m_propertyWidget->getFPSVal();
}

void LayersPanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	m_propertyWidget = new LayersPropertyWidget(this);
	sizer->Add(m_propertyWidget, 0, wxALIGN_LEFT);
	m_contentWidget = new LayersContentWidget(this, wxID_ANY, m_layersMgr);
	sizer->Add(m_contentWidget, 1, wxEXPAND);
	SetSizer(sizer);
}

} // eanim