#ifndef _EASYANIM_VIEW_MGR_H_
#define _EASYANIM_VIEW_MGR_H_

#include <ee/Observer.h>

namespace ee { class LibraryPanel; class PropertySettingPanel; class LibraryPage; class ViewlistPanel; }

namespace eanim
{

class LibraryPanel;
class ToolbarPanel;
class StagePanel;
class LayersPanel;
class KeysPanel;
class KeyFrame;

class ViewMgr : public ee::Observer
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
	LibraryPanel* library;
	ee::PropertySettingPanel* property;

	ee::LibraryPage* img_page;

	// mid
	ToolbarPanel* toolbar;
	StagePanel* stage;

	LayersPanel* layers;
	KeysPanel* keys;

	// right
	ee::ViewlistPanel* viewlist;

private:
	int m_layer, m_frame;

private:
	static ViewMgr* m_instance;

}; // ViewMgr

}

#endif // _EASYANIM_VIEW_MGR_H_