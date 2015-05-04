#ifndef _EASYANIM_CONTROLLER_H_
#define _EASYANIM_CONTROLLER_H_

#include "dataset/LayersMgr.h"
#include "dataset/ResourceMgr.h"

namespace eanim
{

class Widgets;
class KeyFrame;
class KeysPanel;
class LayersPanel;
class ToolbarPanel;
class StagePanel;

class Controller
{
public:
	Controller(Widgets* widgets);

	void setCurrFrame(int layer, int frame);
	KeyFrame* getCurrFrame() { return m_last_keyframe; }
	void setPrevKeyFrame();
	void setNextKeyFrame();

	// layer start from 0
	// frame start from 1
	int layer() const { return m_curr_layer; }
	int frame() const { return m_curr_frame; }

	d2d::ViewlistPanel* GetViewlist();
	d2d::LibraryPanel* GetLibraryPanel();
	StagePanel* GetStagePanel();
	LayersPanel* GetLayersPanel();
	ToolbarPanel* GetToolbarPanel();
	KeysPanel* GetKeysPanel();
	d2d::PropertySettingPanel* GetPropertyPanel();

	void SetTimeLinePanel(LayersPanel* layers_panel, KeysPanel* keys_panel);

	int GetMaxFrame() const { return maxFrame; }
	void SetMaxFrame(int max_frame) { maxFrame = max_frame; }

	void SetKeysPanelPos(int pos);

	void ClearLayers();
	void InsertLayer(Layer* layer);
	LayersMgr& GetLayers() { return layers; }

	d2d::ILibraryPage* GetImagePage();

	ResourceMgr& GetResource() { return resource; }

	const std::string& GetName() const { return name; }

	void Clear();

	void Refresh();

private:
	void reloadViewList(const KeyFrame& frame);

public:
	std::string name;

	wxString packer;

	unsigned int fps;

private:
	Widgets* m_widgets;

	LayersMgr layers;
	int maxFrame;

	ResourceMgr resource;

	//

	KeyFrame* m_last_keyframe;

	// layer start from 0
	// frame start from 1
	int m_curr_layer;
	int m_curr_frame;	

}; // Controller

}

#endif // _EASYANIM_CONTROLLER_H_