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
	int layer = context->layers.size();
	context->setCurrFrame(layer, context->frame());
	context->layers.newLayer();
	m_parent->Refresh();
}

void LayersPanel::removeLayer()
{
	Context* context = Context::Instance();

	static_cast<StagePanel*>(context->stage)->removeSpriteSelection();
	context->layers.removeLayer(context->layer());
	if (context->layers.size() == 0) 
	{
		context->setCurrFrame(-1, context->frame());
	}
	else if (context->layer() > 0) 
	{
		context->setCurrFrame(context->layer() - 1, context->frame());
	}
	static_cast<KeysPanel*>(context->keysPanel)->setCurrPos(context->frame());
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
	sizer->Add(m_contentPanel = new LayersContentWidget(this), 1, wxEXPAND);
	SetSizer(sizer);
}

} // eanim