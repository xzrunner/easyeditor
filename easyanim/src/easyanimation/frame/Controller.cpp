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
	, m_layers(this)
	, m_last_keyframe(NULL)
{
	m_curr_layer = 0;
	m_curr_frame = 1;
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
		m_widgets->m_keysPanel->GetSelectPos(row, col);
		if (row >= 0 && col >= 0) {
			row = GetLayers().Size() - 1 - layer;
			col = m_curr_frame - 1;
			m_widgets->m_keysPanel->SetSelectPos(row, col);
		}
	}

	d2d::SetCanvasDirtySJ::Instance()->SetDirty();
	if (m_widgets->m_keysPanel) {
		m_widgets->m_keysPanel->Refresh(true);
	}
	if (m_widgets->m_layersPanel) {
		m_widgets->m_layersPanel->Refresh(true);
	}

	GetStagePanel()->GetEditOP()->Clear();
}

void Controller::UpdateCurrFrame()
{
	if (m_last_keyframe) {
		return;
	}

	Layer* layer = m_layers.GetLayer(m_curr_layer);
	if (layer) {
		m_last_keyframe = layer->GetCurrKeyFrame(m_curr_frame);
		reloadViewList(m_last_keyframe);
	}
}

KeyFrame* Controller::getCurrFrame() 
{ 
	if (!m_last_keyframe) {
		UpdateCurrFrame();
	}
	return m_last_keyframe; 
}

void Controller::setPrevKeyFrame()
{
	Layer* pLayer = m_layers.GetLayer(m_curr_layer);
	if (!pLayer) {
		return;
	}

	KeyFrame* prev = pLayer->GetPrevKeyFrame(m_curr_frame);
	if (prev) 
	{
		setCurrFrame(m_curr_layer, prev->GetTime());
	}
}

void Controller::setNextKeyFrame()
{
	Layer* pLayer = m_layers.GetLayer(m_curr_layer);
	if (!pLayer) {
		return;
	}

	KeyFrame* next = pLayer->GetNextKeyFrame(m_curr_frame);
	if (next) 
	{
		setCurrFrame(m_curr_layer, next->GetTime());
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

// d2d::ViewlistPanel* Controller::GetViewlist()
// {
// 	return m_widgets->m_viewlist;
// }

d2d::LibraryPanel* Controller::GetLibraryPanel()
{
	return m_widgets->m_library;
}

StagePanel* Controller::GetStagePanel()
{
	return m_widgets->m_stage;
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

int Controller::GetMaxFrame() const 
{ 
	return m_layers.GetMaxFrame();
}

bool Controller::ClearAllLayer()
{
	return m_layers.Clear();
}

d2d::ILibraryPage* Controller::GetImagePage()
{
	return m_widgets->m_imagePage;
}

void Controller::Clear()
{
	m_curr_layer = 0;
	m_curr_frame = 1;
}

void Controller::Refresh()
{
	d2d::SetCanvasDirtySJ::Instance()->SetDirty();
	m_widgets->m_keysPanel->Refresh(true);
}

void Controller::reloadViewList(const KeyFrame* frame)
{
	if (!m_widgets->m_viewlist) return;

	m_widgets->m_viewlist->Clear();
	if (frame)
	{
		const std::vector<d2d::ISprite*>& sprites = frame->GetAllSprites();
		for (int i = 0, n = sprites.size(); i < n; ++i)
			m_widgets->m_viewlist->Insert(sprites[i]);
	}	
}

}