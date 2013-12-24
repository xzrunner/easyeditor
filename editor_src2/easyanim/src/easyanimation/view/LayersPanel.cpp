#include "LayersPanel.h"
#include "LayersPropertyWidget.h"
#include "LayersContentWidget.h"
#include "KeysPanel.h"
#include "StagePanel.h"

#include "frame/Context.h"

namespace eanim
{

LayersPanel::LayersPanel(wxWindow* parent)
	: wxPanel(parent)
{
	initLayout();
	insertLayer();
}

void LayersPanel::insertLayer()
{
	Context* context = Context::Instance();
	context->currLayer = context->layers.size();
	context->layers.newLayer();
	m_parent->Refresh();
}

void LayersPanel::removeLayer()
{
	Context* context = Context::Instance();

	static_cast<StagePanel*>(context->stage)->removeSpriteSelection();
	context->layers.removeLayer(context->currLayer);
	if (context->layers.size() == 0) 
		context->currLayer = -1;
	else if (context->currLayer > 0) 
		--context->currLayer;
	static_cast<KeysPanel*>(context->keysPanel)->setCurrPos(context->currFrame);
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
	sizer->Add(m_layersProperty = new LayersPropertyWidget(this), 0, wxALIGN_LEFT);
	sizer->Add(new LayersContentWidget(this), 1, wxEXPAND);
	SetSizer(sizer);
}

} // eanim