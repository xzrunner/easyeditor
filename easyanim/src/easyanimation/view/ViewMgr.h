#ifndef _EASYANIM_VIEW_MGR_H_
#define _EASYANIM_VIEW_MGR_H_

#include <drag2d.h>

namespace eanim
{

class ToolbarPanel;
class StagePanel;
class LayersPanel;
class KeysPanel;

class ViewMgr
{
public:	
	static ViewMgr* Instance();

private:
	ViewMgr();

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
	static ViewMgr* m_instance;

}; // ViewMgr

}

#endif // _EASYANIM_VIEW_MGR_H_