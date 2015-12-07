#ifndef _EASYANIM_VIEW_MGR_H_
#define _EASYANIM_VIEW_MGR_H_

#include <drag2d.h>

namespace eanim
{

class ToolbarPanel;
class StagePanel;
class LayersPanel;
class KeysPanel;
class KeyFrame;

class ViewMgr : public d2d::Observer
{
public:	
	static ViewMgr* Instance();

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	ViewMgr();

private:
	void UpdateProperty(KeyFrame* frame);
	void UpdateViewlist(KeyFrame* frame);

public:
	// left
	d2d::LibraryPanel* library;
	d2d::PropertySettingPanel* property;

	d2d::ILibraryPage* img_page;

	// mid
	ToolbarPanel* toolbar;
	StagePanel* stage;

	LayersPanel* layers;
	KeysPanel* keys;

	// right
	d2d::ViewlistPanel* viewlist;

private:
	int m_layer, m_frame;

private:
	static ViewMgr* m_instance;

}; // ViewMgr

}

#endif // _EASYANIM_VIEW_MGR_H_