#include "Controller.h"
#include "Widgets.h"

#include "dataset/KeyFrame.h"
#include "dataset/Layer.h"
#include "view/KeysPanel.h"
#include "view/StagePanel.h"
#include "view/LayersPanel.h"

namespace eanim
{

Controller::Controller(Widgets* widgets)
	: m_widgets(widgets)
	, layers(this)
	, m_last_keyframe(NULL)
{
	m_curr_layer = 0;
	m_curr_frame = 1;
	maxFrame = -1;

	fps = 30;
}

void Controller::setCurrFrame(int layer, int frame)
{
	if (m_curr_layer == layer &&
		m_curr_frame == frame) {
		return;
	}

	m_last_keyframe = NULL;

	m_curr_layer = layer;
	m_curr_frame = frame;

	if (m_widgets->m_keysPanel) 
	{
		int row, col;
		m_widgets->m_keysPanel->getSelectPos(row, col);
		if (row >= 0 && col >= 0) {
			col = m_curr_frame - 1;
			m_widgets->m_keysPanel->setSelectPos(row, col);
		}
	}

	m_widgets->m_stage->Refresh();
	if (m_widgets->m_keysPanel) {
		m_widgets->m_keysPanel->Refresh();
	}
	if (m_widgets->m_layersPanel) {
		m_widgets->m_layersPanel->Refresh();
	}
}

KeyFrame* Controller::getCurrFrame() 
{
	if (!m_last_keyframe)
	{
		Layer* layer = layers.getLayer(m_curr_layer);
		if (layer) {
			m_last_keyframe = layer->getCurrKeyFrame(m_curr_frame);
			reloadViewList(m_last_keyframe);
		}
	}
	return m_last_keyframe; 
}

void Controller::setPrevKeyFrame()
{
	Layer* pLayer = layers.getLayer(m_curr_layer);
	if (!pLayer) {
		return;
	}

	KeyFrame* prev = pLayer->getPrevKeyFrame(m_curr_frame);
	if (prev) 
	{
		setCurrFrame(m_curr_layer, prev->getTime());
	}
}

void Controller::setNextKeyFrame()
{
	Layer* pLayer = layers.getLayer(m_curr_layer);
	if (!pLayer) {
		return;
	}

	KeyFrame* next = pLayer->getNextKeyFrame(m_curr_frame);
	if (next) 
	{
		setCurrFrame(m_curr_layer, next->getTime());
	}
}

int Controller::layer() const 
{ 
	return m_curr_layer; 
}

int Controller::frame() const 
{ 
	return m_curr_frame; 
}

d2d::ViewlistPanel* Controller::GetViewlist()
{
	return m_widgets->m_viewlist;
}

d2d::LibraryPanel* Controller::GetLibraryPanel()
{
	return m_widgets->m_library;
}

StagePanel* Controller::GetStagePanel()
{
	return m_widgets->m_stage;
}

LayersPanel* Controller::GetLayersPanel()
{
	return m_widgets->m_layersPanel;
}

ToolbarPanel* Controller::GetToolbarPanel()
{
	return m_widgets->m_toolbar;
}

KeysPanel* Controller::GetKeysPanel()
{
	return m_widgets->m_keysPanel;
}

d2d::PropertySettingPanel* Controller::GetPropertyPanel()
{
	return m_widgets->m_property;
}

void Controller::SetTimeLinePanel(LayersPanel* layers_panel, KeysPanel* keys_panel)
{
	m_widgets->m_layersPanel = layers_panel;
	m_widgets->m_keysPanel = keys_panel;
}

void Controller::SetKeysPanelPos(int pos)
{
	m_widgets->m_keysPanel->setCurrPos(pos);
}

void Controller::ClearLayers()
{
	layers.clear();
}

void Controller::InsertLayer(Layer* layer)
{
	layers.insertLayer(layer);
}

d2d::ILibraryPage* Controller::GetImagePage()
{
	return m_widgets->m_imagePage;
}

void Controller::Clear()
{
	m_widgets->m_library->clear();
	m_widgets->m_stage->clear();
	ClearLayers();
}

void Controller::Refresh()
{
	m_widgets->m_stage->Refresh();
	m_widgets->m_keysPanel->Refresh();
}

void Controller::reloadViewList(const KeyFrame* frame)
{
	if (!m_widgets->m_viewlist) return;

	m_widgets->m_viewlist->clear();
	if (frame)
	{
		const std::vector<d2d::ISprite*>& sprites = frame->getAllSprites();
		for (int i = 0, n = sprites.size(); i < n; ++i)
			m_widgets->m_viewlist->insert(sprites[i]);
	}	
}

}