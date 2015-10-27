#ifndef _EASYUI_TOP_PANNELS_H_
#define _EASYUI_TOP_PANNELS_H_

#include <drag2d.h>

namespace eui
{

class LibraryPanel;
class StagePanel;
class ToolbarPanel;

class TopPannels
{
public:
	d2d::ViewPanelMgr view_panel_mgr;

	LibraryPanel* library;

	d2d::PropertySettingPanel* property;

	StagePanel* stage;

	ToolbarPanel* toolbar;

public:
	TopPannels();

	void Clear();

}; // TopPannels

}

#endif // _EASYUI_TOP_PANNELS_H_
