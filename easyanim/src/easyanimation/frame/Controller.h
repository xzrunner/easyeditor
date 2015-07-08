#ifndef _EASYANIM_CONTROLLER_H_
#define _EASYANIM_CONTROLLER_H_

#include "dataset/LayersMgr.h"
#include "dataset/AnimTemplate.h"

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
	void UpdateCurrFrame();
	KeyFrame* getCurrFrame();
	void setPrevKeyFrame();
	void setNextKeyFrame();

	// layer start from 0
	// frame start from 1
	int layer() const;
	int frame() const;

	d2d::ViewlistPanel* GetViewlist();
	d2d::LibraryPanel* GetLibraryPanel();
	StagePanel* GetStagePanel();
	LayersPanel* GetLayersPanel();
	ToolbarPanel* GetToolbarPanel();
	KeysPanel* GetKeysPanel();
	d2d::PropertySettingPanel* GetPropertyPanel();

	void SetTimeLinePanel(LayersPanel* layers_panel, KeysPanel* keys_panel);

	int GetMaxFrame() const;

	bool ClearAllLayer();
	void InsertLayer(Layer* layer);
	LayersMgr& GetLayers() { return layers; }

	d2d::ILibraryPage* GetImagePage();

	AnimTemplate& GetAnimTemplate() { return anim_template; }

	const std::string& GetName() const { return name; }

	void Clear();

	void Refresh();

	void reloadViewList(const KeyFrame* frame);

public:
	std::string name;

	wxString packer;

	unsigned int fps;

private:
	Widgets* m_widgets;

	LayersMgr layers;

	AnimTemplate anim_template;

	//

	KeyFrame* m_last_keyframe;

	// layer start from 0
	// frame start from 1
	int m_curr_layer;
	int m_curr_frame;	

	struct FramePos
	{
		int row, col;
	};
	d2d::SelectionSet<FramePos> m_selected_frame;

}; // Controller

}

#endif // _EASYANIM_CONTROLLER_H_